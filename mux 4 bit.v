module st_mux4b(output Y, input [3:0] D, input [1:0] A);
    wire [5:0] w;
    not(w[1], A[1]);
    not(w[0], A[0]);
    and(w[2], A[0], A[1], D[3]);
    and(w[3], w[0], A[1], D[2]);
    and(w[4], A[0], w[1], D[1]);
    and(w[5], w[0], w[1], D[0]);
    or(Y, w[2], w[3], w[4], w[5]);
endmodule //st_mux4b
/*
module st_mux4b_tb;
    reg[3:0] data;
    reg[1:0] addr;
    wire out;
    integer i,j;
    initial begin
        $dumpfile("st_mux4b");
        $dumpvars;
        data=0;
        addr=0;
        for(i=0; i<16; ++i) begin
            for(j=0; j<4; ++j) begin
                #5 data=i;
                   addr=j;
            end
        end
    #20 $finish;
    end
    st_mux4b mymux(y,data,addr);
endmodule // st_mux4b_tb
*/

module be_mux4b(output y, input[3:0] d, input[1:0] a);
    assign y = ((~a[0]) & (~a[1])) & d[0] | (a[0] & (~a[1])) & d[1] | ((~a[0]) & a[1]) & d[2] | (a[0] & a[1]) & d[3];
endmodule

module be_mux4b_tb;
    reg[3:0] data;
    reg[1:0] addr;
    wire out;
    integer i,j;
    initial begin
        $dumpfile("be_mux4b");
        $dumpvars;
        data=0;
        addr=0;
        for(i=0; i<16; ++i) begin
            for(j=0; j<4; ++j) begin
                #5 data=i;
                   addr=j;
            end
        end
    #20 $finish;
    end
    be_mux4b mymux(y,data,addr);
endmodule // be_mux4b_tb
