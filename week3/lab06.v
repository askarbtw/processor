{\rtf1\ansi\ansicpg1252\cocoartf2757
\cocoatextscaling0\cocoaplatform0{\fonttbl\f0\fswiss\fcharset0 Helvetica;}
{\colortbl;\red255\green255\blue255;}
{\*\expandedcolortbl;;}
\paperw11900\paperh16840\margl1440\margr1440\vieww11520\viewh8400\viewkind0
\pard\tx720\tx1440\tx2160\tx2880\tx3600\tx4320\tx5040\tx5760\tx6480\tx7200\tx7920\tx8640\pardirnatural\partightenfactor0

\f0\fs24 \cf0 module priority_encoder_9to4 (\
    input wire in8,\
    input wire in7,\
    input wire in6,\
    input wire in5,\
    input wire in4,\
    input wire in3,\
    input wire in2,\
    input wire in1,\
    input wire in0,\
\
    output reg [3:0] out\
);\
\
always @(*) begin\
    if (in8)\
        out = 4'b1001;\
    else if (in7)\
        out = 4'b1000;\
    else if (in6)\
        out = 4'b0111;\
    else if (in5)\
        out = 4'b0110;\
    else if (in4)\
        out = 4'b0101;\
    else if (in3)\
        out = 4'b0100;\
    else if (in2)\
        out = 4'b0011;\
    else if (in1)\
        out = 4'b0010;\
    else if (in0)\
        out = 4'b0001;\
    else\
        out = 4'b0000;\
end\
\
endmodule}
