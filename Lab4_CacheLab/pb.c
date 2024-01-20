char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{  
    if(M == 32 && N == 32){
	int i, bi, bj;
	int tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7, tmp8;
	for(bi = 0;bi < M;bi += 8){
		for(bj = 0;bj < N;bj+=8){
			for(i = bi;i < bi+8;i++){
				tmp1 = A[i][0+bj]; 
				tmp2 = A[i][1+bj]; 
				tmp3 = A[i][2+bj]; 
				tmp4 = A[i][3+bj];
				tmp5 = A[i][4+bj];
			       	tmp6 = A[i][5+bj];
			       	tmp7 = A[i][6+bj];
			       	tmp8 = A[i][7+bj];

				B[0+bj][i] = tmp1;
			       	B[1+bj][i] = tmp2;
			       	B[2+bj][i] = tmp3;
			       	B[3+bj][i] = tmp4;
				B[4+bj][i] = tmp5;
			       	B[5+bj][i] = tmp6;
			       	B[6+bj][i] = tmp7;
			       	B[7+bj][i] = tmp8; 
			}		
		}
	}
   }else if(M == 64 && N == 64){
	int i, j, bi, bj;
	int tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7, tmp8;
	for (bi = 0; bi < N; bi += 8) {
    		for (bj = 0; bj < M; bj += 8) {
        		for (i = bi; i < bi + 4; i++) {
            			tmp1 = A[i][0+bj];
			       	tmp2 = A[i][1+bj];
			       	tmp3 = A[i][2+bj];
			       	tmp4 = A[i][3+bj];  
				tmp5 = A[i][4+bj];
			       	tmp6 = A[i][5+bj];
			       	tmp7 = A[i][6+bj];
			       	tmp8 = A[i][7+bj];

            			B[0+bj][i] = tmp1;
			       	B[1+bj][i] = tmp2;
			       	B[2+bj][i] = tmp3;
			       	B[3+bj][i] = tmp4;              
            			B[0+bj][4+i] = tmp5;
			       	B[1+bj][4+i] = tmp6;
			       	B[2+bj][4+i] = tmp7;
			       	B[3+bj][4+i] = tmp8;  
        		}
        		for (j = bj; j < bj + 4; j++) {
            			tmp1 = A[4+bi][j];
			       	tmp2 = A[5+bi][j];
			       	tmp3 = A[6+bi][j];
			       	tmp4 = A[7+bi][j];
            			tmp5 = B[j][4+bi];
			       	tmp6 = B[j][5+bi];
			       	tmp7 = B[j][6+bi];
			       	tmp8 = B[j][7+bi];

            			B[j][4+bi] = tmp1;
			       	B[j][5+bi] = tmp2;
			       	B[j][6+bi] = tmp3;
			       	B[j][7+bi] = tmp4;          
            			B[4+j][0+bi] = tmp5;
			       	B[4+j][1+bi] = tmp6;
			       	B[4+j][2+bi] = tmp7;
			       	B[4+j][3+bi] = tmp8; 
        		}
        		for (i = bi + 4; i < bi + 8; i++) {
           			 tmp1 = A[i][4+bj];
				 tmp2 = A[i][5+bj];
				 tmp3 = A[i][6+bj];
				 tmp4 = A[i][7+bj]; 

            			 B[4+bj][i] = tmp1;
				 B[5+bj][i] = tmp2;
				 B[6+bj][i] = tmp3;
				 B[7+bj][i] = tmp4; 
        		}
    		}
	} 	
   }else if(M == 61 && N == 67){
   		int bi, bj, i, j, tmp;
		int block_size = 16;
		for (bi = 0; bi < N; bi += block_size) {
    		for (bj = 0; bj < M; bj += block_size) {
        		for (i = bi; i < N && i < bi + block_size; i++) {
            			for (j = bj; j < M && j < bj + block_size; j++) {
                			tmp = A[i][j];
                			B[j][i] = tmp;
            			}
        		}
    		}
		}	
   }
}