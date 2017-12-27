clc
echo off

% GF(2^3)生成矩阵
GenerMatix = [1 0 0;
                        0 1 0;
                        0 0 1;
                        6 4 3; 
                        1 1 1;
                        6 5 2;
                        7 5 3];
                    
 Remain_GenerateMatrix = GenerMatix;

% GF(2^3)的校验矩阵
 HMatix =  [ 6 4 3 1 0 0 0;
                    1 1 1 0 1 0 0;
                    6 5 2 0 0 1 0;
                    7 5 3 0 0 0 1];

 disp('原始的信息位：')               
 M = [6;  5;  4] % M是信息位
 
 M = gf(M, 3);
 add_v = gf(add_v, 3);
 multi_v = gf(multi_v, 3);
 GenerMatix = gf(GenerMatix, 3);
 HMatix = gf(HMatix, 3);
 

 after_M =  GenerMatix * M ;
 disp('经过编码后的信息为')
 after_M
 
error_Loc = randperm(7);  %生成1到7的随机排列 模拟发生块错误
disp('随机生成几块错误')
error_Loc = error_Loc(1:4) %取前4个

disp('生成错误后 剩余的生成矩阵为')
Remain_GenerateMatrix(error_Loc,:)=[]

after_M=double(after_M.x); %将伽罗华域中的元素 转换成数域里面
disp('生成错误后 剩余的信息矩阵为')
after_M(error_Loc,:)=[]

Remain_GenerateMatrix = gf(Remain_GenerateMatrix, 3);
after_M = gf(after_M, 3);
disp('修正后的信息为：')
inv(Remain_GenerateMatrix) * after_M
 
 
 
 
 
 
 
 
 
 
 
 
 
 
        