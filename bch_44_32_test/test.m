clc
clear all 
%------------
n = 63;
m = 6;
t = 2;
a = gf(2, 6); %构建扩域，matlab自动只把a当作扩域中的本原元
for i = 1 : 2 :(2 * t -1) %求最小多项式，只找奇数项
     b = [1, a ^ i]; %让b分别为a ^ i..找每一个（这么定义是要卷积的）
     l = i;
     while a ^ i ~= a^(2 * l) %找共轭根系
         l = 2 * l;
         b = conv(b, [1, a ^ l]); % 求最小多项式
     end
     if i == 1
         g = b;
     else
         g = conv(g, b); %求生成多项式 利用卷积，进行连乘
     end
end
g.x;
gx = double(g.x) %从扩域到数域 相当于变成多项式 
k = n - length(gx) + 1; % k = n - (r + 1) + 1 k即为编码的长度 BCH的性质 由纠错的个数来决定生成多项式， 然后定义相关的码长，最后才生成信息位
disp(['计算得码长为 k  = ', num2str(k)]);

syms x;
for i = 1 : k  %生成K行
    m1 = x ^ (n - i);
    m2 = sym2poly(m1); %多项式的系数 从高位到低位 返回一个向量
    [Q, R] = deconv(m2  , gx); %可以看成相除得到的余数，q为商，r为余数 ，返回两个向量
    R=mod(R,2); %由于多项式进行的是 模2运算，上面求出来真正的余数为R， 然后对2进行取余 ？ （这里我，。看的是结果正确，跟师兄的结果是一样的。但也是不太确定。）
    R = R(end - 11 :  end) %只取最后的12位，如果是BCH(44， 32),标准的生成矩阵应该为 R[32 * 44] ,其中32 * 32为单位矩阵，32 * 12为相应的校验
    %现在每一行生成的相当于 生成矩阵的 右半部分，即 r（x） =  (x ^ n - i) % g（x），左半部分为单位矩阵，进行拼接即可
end

H = [1 0 1 0 1 0 0 1 1 1 0 0; 
0 1 0 1 0 1 0 0 1 1 1 0; 
0 0 1 0 1 0 1 0 0 1 1 1; 
1 0 1 1 1 1 0 0 1 1 1 1; 
1 1 1 1 0 1 1 1 1 0 1 1; 
1 1 0 1 0 0 1 0 0 0 0 1; 
1 1 0 0 0 0 0 0 1 1 0 0; 
0 1 1 0 0 0 0 0 0 1 1 0; 
0 0 1 1 0 0 0 0 0 0 1 1; 
1 0 1 1 0 0 0 1 1 1 0 1; 
1 1 1 1 0 0 0 1 0 0 1 0; 
0 1 1 1 1 0 0 0 1 0 0 1; 
1 0 0 1 0 1 0 1 1 0 0 0; 
0 1 0 0 1 0 1 0 1 1 0 0; 
0 0 1 0 0 1 0 1 0 1 1 0; 
0 0 0 1 0 0 1 0 1 0 1 1; 
1 0 1 0 0 0 0 0 1 0 0 1; 
1 1 1 1 1 0 0 1 1 0 0 0; 
0 1 1 1 1 1 0 0 1 1 0 0; 
0 0 1 1 1 1 1 0 0 1 1 0; 
0 0 0 1 1 1 1 1 0 0 1 1; 
1 0 1 0 0 1 1 0 0 1 0 1; 
1 1 1 1 1 0 1 0 1 1 1 0; 
0 1 1 1 1 1 0 1 0 1 1 1; 
1 0 0 1 0 1 1 1 0 1 1 1; 
1 1 1 0 0 0 1 0 0 1 1 1; 
1 1 0 1 1 0 0 0 1 1 1 1; 
1 1 0 0 0 1 0 1 1 0 1 1; 
1 1 0 0 1 0 1 1 0 0 0 1; 
1 1 0 0 1 1 0 0 0 1 0 0; 
0 1 1 0 0 1 1 0 0 0 1 0; 
0 0 1 1 0 0 1 1 0 0 0 1] 

H'

eye(12)





















