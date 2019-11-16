const MAXWORD=50;
const DICTSIZ=100;

typedef int int_ptr<>;

struct matrix {
    int_ptr data;
    int nrow;
    int ncol;
};

program RDICTPROG {
	version RDICTVERS {
        int INITW(void)=1;
        int INSERTW(string)=2;
        int_ptr ARRAY_EXAMPLE(int_ptr, int_ptr) = 3;
        matrix MATRIX_EXAMPLE(matrix) = 4;
	} =1;
} =0x30090949;
