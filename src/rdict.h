/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#ifndef _RDICT_H_RPCGEN
#define _RDICT_H_RPCGEN

#include <rpc/rpc.h>

#include <pthread.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MAXWORD 50
#define DICTSIZ 100

typedef struct {
	u_int int_ptr_len;
	int *int_ptr_val;
} int_ptr;

struct matrix {
	int_ptr data;
	int nrow;
	int ncol;
};
typedef struct matrix matrix;

struct array_example_1_argument {
	int_ptr arg1;
	int_ptr arg2;
};
typedef struct array_example_1_argument array_example_1_argument;

#define RDICTPROG 0x30090949
#define RDICTVERS 1

#if defined(__STDC__) || defined(__cplusplus)
#define INITW 1
extern  enum clnt_stat initw_1(int *, CLIENT *);
extern  bool_t initw_1_svc(int *, struct svc_req *);
#define INSERTW 2
extern  enum clnt_stat insertw_1(char *, int *, CLIENT *);
extern  bool_t insertw_1_svc(char *, int *, struct svc_req *);
#define ARRAY_EXAMPLE 3
extern  enum clnt_stat array_example_1(int_ptr , int_ptr , int_ptr *, CLIENT *);
extern  bool_t array_example_1_svc(int_ptr , int_ptr , int_ptr *, struct svc_req *);
#define MATRIX_EXAMPLE 4
extern  enum clnt_stat matrix_example_1(matrix , matrix *, CLIENT *);
extern  bool_t matrix_example_1_svc(matrix , matrix *, struct svc_req *);
extern int rdictprog_1_freeresult (SVCXPRT *, xdrproc_t, caddr_t);

#else /* K&R C */
#define INITW 1
extern  enum clnt_stat initw_1();
extern  bool_t initw_1_svc();
#define INSERTW 2
extern  enum clnt_stat insertw_1();
extern  bool_t insertw_1_svc();
#define ARRAY_EXAMPLE 3
extern  enum clnt_stat array_example_1();
extern  bool_t array_example_1_svc();
#define MATRIX_EXAMPLE 4
extern  enum clnt_stat matrix_example_1();
extern  bool_t matrix_example_1_svc();
extern int rdictprog_1_freeresult ();
#endif /* K&R C */

/* the xdr functions */

#if defined(__STDC__) || defined(__cplusplus)
extern  bool_t xdr_int_ptr (XDR *, int_ptr*);
extern  bool_t xdr_matrix (XDR *, matrix*);
extern  bool_t xdr_array_example_1_argument (XDR *, array_example_1_argument*);

#else /* K&R C */
extern bool_t xdr_int_ptr ();
extern bool_t xdr_matrix ();
extern bool_t xdr_array_example_1_argument ();

#endif /* K&R C */

#ifdef __cplusplus
}
#endif

#endif /* !_RDICT_H_RPCGEN */
