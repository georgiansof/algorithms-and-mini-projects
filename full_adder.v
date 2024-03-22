module st_fa(output Co, output S, input A, input B, input Ci);
    wire w1,w2,w3;
    xor (S,A,B,Ci);
    and (w1,A,B);
    or(w2,A,B);
    and(w3,Ci,w2);
    or(Co,w1,w3);
endmodule // st_fa
/*
module st_fa_tb();
    reg a,b,ci;
    wire co,s;
    initial begin
        $dumpfile("st_fa");
        $dumpvars;
        #5 a=0;
        #5 b=0;
        #5 ci=0;
        #5 ci=1;
        #5 b=1;
        #5 ci=0;
        #5 a=1;
        #5 ci=1;
        #5 b=0;
        #5 ci=0;
        #5 $finish;
    end
    
    st_fa myfa(co,s,a,b,ci);
    
endmodule // st_fa_tb
*/
module st_sum4(output Co, output [3:0] S,input [3:0] A,input [3:0] B,input Ci);
    wire [2:0] w;
    st_fa lsb (w[0],S[0],A[0],B[0],Ci);
    st_fa bit1(w[1],S[1],A[1],B[1],w[0]);
    st_fa bit2(w[2],S[2],A[2],B[2],w[1]);
    st_fa msb (Co,S[3],A[3],B[3],w[2]);
endmodule // st_sum4

module st_sum4_tb();
    reg ci;
    reg [3:0] a,b;
    wire [3:0] s;
    wire co;
    integer i;
    initial begin
        $dumpfile("st_sum4");
        $dumpvars;
        a  = 0;
        b  = 0;
        ci = 0;
        for(i=0; i<16; ++i) begin
            #5 a=$random;
            b=$random;
            ci=$random;
        end 
        #10 $finish;
    end 

    st_sum4 my_st_sum4(co,s,a,b,ci);
endmodule //st_sum4_tb
    
    
module be_fa(Co,S,A,B,Ci);
    output Co,S;
    input A,B,Ci;
    assign {Co,S} = A + B + Ci;
endmodule // be_fa

module be_sum4(Co,S,A,B,Ci);
    output Co;
    output [3:0] S;
    input [3:0] A,B;
    input Ci;
    assign {Co,S} = A + B + Ci;
endmodule // be_sum4

/*
module be_sum4_tb();
    reg ci;
    reg [3:0] a,b;
    wire [3:0] s;
    wire co;
    integer i;
    initial begin
        $dumpfile("be_sum4");
        $dumpvars;
        a  = 0;
        b  = 0;
        ci = 0;
        for(i=0; i<16; ++i) begin
            #5 a=$random;
            b=$random;
            ci=$random;
        end 
        #10 $finish;
    end 

    be_sum4 my_be_sum4(co,s,a,b,ci);
    endmodule //st_sum4_tb
*/

/*
module be_fa_tb();
    reg a,b,ci;
    wire co,s;
    initial begin
        $dumpfile("be_fa");
        $dumpvars;
        #5 a=0;
        #5 b=0;
        #5 ci=0;
        #5 ci=1;
        #5 b=1;
        #5 ci=0;
        #5 a=1;
        #5 ci=1;
        #5 b=0;
        #5 ci=0;
        #5 $finish;
    end
    be_fa my_be_fa(co,s,a,b,ci);
endmodule*/
