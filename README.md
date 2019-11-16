# rpcgen-multithreaded
An implementation of multi-threaded rpcgen code suitable for compiling as 
rpcgen -a -M 


You can build this project in 2 ways.  

**With Cmake**

First, using cmake (and c++)
create a directory named 'build' in the root dir...

    cd build
    cmake ..

Next run the server

    ./server

Then run the client

    ./client
    
**With Makefile**

The second way you can build is to enter the src directory and use the 
include make file.

    make clean
    make
    
Run the server with 
 
    make run_server

Run the client with

    make run_client
    
    
The above builds have been tested with gcc version 7.4.0 on Ubuntu 
7.4.0-1ubuntu1~18.04.1l.

The two builds are here for different reasons, but essentially accomplish the
same thing (until I start modifying things).

The cmake based build enables c++ code to be mixed in, while the makefile 
build uses the rpcgen built in makefile which does not compile or link with
c++ stuff.  You can the rpcgen Makefile.rdict file to compile with c++ support
by adding the following defs somewhere above the object rules in the makefile.

    CC=g++
    CXX=g++
    





## What is going on here?

Using rpcgen on Linux is somewhat of a pain.  There are several people who
have stack overflow threads and a few other mentions on the internet where
basically everyone says "why are you using rpcgen... move on," and then 
talks about how rpcgen is only partially supported on linux.  In fact, those
people are mostly right.  Looking at the man page, you can clearly see
that the linux variant does not support the -A or the -M flags, which are 
the two core flags you need to use rpcgen to generate multi-threaded code.

See the man page here...
https://linux.die.net/man/1/rpcgen

However, after being given an assignment which required using rpcgen to 
create concurrent code, I scowered the net and found the following 
http://www.redaht.com/archives/redhat-list/2004-June/msg00439.html . 

Note that in the above, the author, "Boris Derzhavets" publised the code with
the following message...

"This message is supposed to give a positive answer for the question 23.10
from [1] Chapter "RPC". Actually code published in [1] utilizes static
variables   what causes question 23.10 from [1](about multihreaded
implementation   of RPC Server under Linux) to produce some confusion.
Originally only two files are taken from [1]: rdict.x  and rdict_srp.c. All
business logic is implemented into rdict_client.c - file generated by
command:
    $ rpcgen -a -M rdict.x
Files rdict.c and rdict_cif.c (see [1] , Chapter "RPC") are taken out to
highlight the core of RPC technology either "Sun" or "DCE"
Remember -A option of rpcgen is not supported under Linux. Library calls
providing by SunOS RPC to build Multithreaded RPC Server are unavailable 
under Linux as well. I believe that code has been developed could be very
helpful for Linux developers at this time. The example given bellow 
demonstrate the universal approach to build multithreaded RPC Severs."

I'm not clear as to what Chapter he is talking about or question he is responding
to, but But, his code gives us a workable starting point from which we can get 
started.  

The first key element to note is that he wrote some code into the rdict_svc.c file, 
which rpcgen states should not be modified. 

After looking at the changes he made, I realized that he basically intercepted
the rdictprog_1(...) function call out of main in rdict_svc.c.  Afterwards, he
builds a C style context for the subsequent launch of a pthread.  Notice the 
struct he builds... 

    struct data_str {
        struct svc_req *rqstp;
        SVCXPRT *transp;
    } *data_ptr=(struct data_str*)malloc(sizeof(struct data_str));
    
The above is an interesting way to define and simeotaneously create an instance 
of a C structure on the fly.  I've actually never seen this before but yeah...

So after he creates that thing, he stashes the rqstp and transp variables which
were created in the main function into it, and then launches what would normally
be generated as the primary program entry point (but he renamed to serv_request)
from a pthread in the following lines... 

    data_ptr-> rqstp = rqstp;
    data_ptr-> transp = transp;
    pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
    pthread_create(&p_thread,&attr,serv_request,(void *)data_ptr);

Notice how he casts the data_ptr to a void* .  This is required for passing arguments
to a new thread under pthreads. 

When the thread is launched, you pass the name of the function you want to lanch 
"serv_request" as the third argument, and then the context data you passed will 
be accessible through the void*.  You can see that he decomposes that void star
back into the rqstp and transp pointers in the following lines... 


    struct thr_data {
        struct svc_req *rqstp;
        SVCXPRT *transp;
    } *ptr_data;

    ptr_data = (struct thr_data  *)data;
    struct svc_req *rqstp = ptr_data-> rqstp;
    register SVCXPRT *transp = ptr_data-> transp;

The above of course is really goofy because he isn't using the same structure as
he created when he first made the context as he is above.  But alas, this is 
just a convoluted way to get the rqstp and transp back from the void* data. 

With all of the above in mind, we can finally understand that he was just packing
some data up, launching a thread, and passing that data to the thread... No big deal.
:-)  He needed to do that likely because the Linux rpcgen is missing functionality
which normally would have been built in a old fashioned SunOS version of rpcgen, but
was just not ported over to linux like it should have been.  

Some further changes the author made were to change the names of some of the files 
that rpcgen creates to other names ... not sure why but that is what he did.  

If you use the diff tool after you generate from your .x file, you can see how he 
modified the files in question. 


# What have I done here.  

SO... a big problem I had when I first started to mess with the authors code was that
I didn't understand the memory model, and the author's example only shows what happens
when you accept a string as a parameter and return an int...

The authors rdict.x file is below...

    const MAXWORD=50;
    const DICTSIZ=100;
    struct example{
        int exfield1;
        char exfield2;
    };
    program RDICTPROG {
        version RDICTVERS {
        int INITW(void)=1;
        int INSERTW(string)=2;
        int DELETEW(string)=3;
        int LOOKUPW(string)=4;
        } =1;
    } =0x30090949;

Turns out that if you know anything about how rpcgen is supposed to work, returning other
types isn't a big deal.  There are a few key things to consider when getting more complex 
types to work (like arrays). 

  1. Allocate on both sides.  You should malloc memory on both the client and the server side.
  2. Use only xdr types.  For example, don't bother with int* or char* in your .x file.
  3. Typedef your complex types... for example an int * should be typedef int int_ptr<>.

A valid type for a 2D array might look something like the following ...

    typedef int int_ptr<>;

    struct matrix {
        int_ptr data;
        int nrow;
        int ncol;
    };
    
Note that you will have to use 1D access patterns.  If you aren't familiar with doing
2D indexing on a 1D data structure, then you will have problems here.

The next thing to note is that in your server.c code, you will get your arguments in
one of two ways...

The -N flag will cause you to be able to pass multiple arguements by value into the header.
Without it, you will have to pack all your arguments into a single structure and then 
unpack them yourself.  Additionally, you **MUST BE AWARE** that when you receive an 
arguement like an int_ptr from above, the rpcgen system will automatically create a 
structure for your arguement.  They won't tell you that they changed the data type, but
you can read about this stuff in documentation I have found scattered on the web...

For example if you had .x as follows 

    typedef int int_ptr<>;
    
    program A3_PROG {
        version A3_VERS {
            int_ptr SORT_ARRAY(int_ptr) = 2;
        } =1;
    } =0x30090949;

and assumming that you compiled the above with flags -a -N -M, rpcgen would
create a header in the _server.h as follows...

    bool_t sort_array_1_svc(int_ptr arg1, int_ptr *result, struct svc_req *rqstp){
        ...
    }

Note that you asked for an int_ptr, but inside of this function, the int_ptr is actually
a structure that looks like the follwing. 

    struct int_ptr {
        int * int_ptr_val;
        int int_ptr_len; 
    }

It was automatically generated for you by rpcgen and you **MUST** fill it out and allocate
directly into it for it to be useful. 


## Who owns the copyright?
I don't know.  I make no claims to the code above.  It is my assumption that the author 
mentioned above owns this code.  

## What I modified.  
I didn't like the way the author was packing his void * in the pthread launch.  So I 
rewrote the relevant code.  I also didn't like that the author was renaming files.  In
order for me to keep it straigt in my head what was going on, I moved some code around
to make it go back into the files that are generated by rpcgen naturally.  

I additionally add an example which shows how you can use one of the int_ptr types I 
mentioned above, and a further example with a nested int_ptr structure that I called
a 'matrix'.  Hopefully these examples will help people see how to handle a more varied
typeset, and to potentially use this for something real (although why when there is 
plenty of modern rpc's to choose from?).  

I added a bunch of comments, and ripped out all the authors menu code.  In my client
it is hardcoded to just call 4 functions which pass data back and forth to and from 
the server. 


## Can someone show me how to make an example with a linked list?
Also, if anyone has any comments on anything weird going on with my memory management, 
let me know. 
