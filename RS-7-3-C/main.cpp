#include <iostream>
#include <iomanip>
#include <cmath>
#include <stdio.h>
#include <vector>
#include <stdlib.h>
using namespace std;

void print(int (*x)[3], int row_size); //打印出传给的矩阵
int fun_addAndSubtract(int a, int b); //有限域内加法和减法（都是基于异或的）
int fun_Multiply(int a, int b); //有限域内的乘法（基于查表的）
int fun_divide(int a, int b); //有限域内的除法（基于反查表的）
int fun_GaussBaseGf(int (*a)[3], int row_size); //实现高斯消元 求解矩阵的逆

//想实现 将伽罗华域映射为柯西矩阵
//最后寻找最优的柯西矩阵使总的运算量 最小
//先生成伽罗华的加和乘的规则
const vector<vector<int> > add_v = {
    {0, 1, 2, 3, 4, 5, 6, 7},
    {1, 0, 3, 2, 5, 4, 7, 6},
    {2, 3, 0, 1, 6, 7, 4, 5},
    {3, 2, 1, 0, 7, 6, 5, 4},
    {4, 5, 6, 7, 0, 1, 2, 3},
    {5, 4, 7, 6, 1, 0, 3, 2},
    {6, 7, 4, 5, 2, 3, 0, 1},
    {7, 6, 5, 4, 3, 2, 1, 0}
};
const vector<vector<int> > multi_v = {
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 1, 2, 3, 4, 5, 6, 7},
    {0, 2, 4, 6, 3, 1, 7, 5},
    {0, 3, 6, 5, 7, 4, 1, 2},
    {0, 4, 3, 7, 6, 2, 5, 1},
    {0, 5, 1, 4, 2, 7, 3, 6},
    {0, 6, 7, 1, 5, 3, 2, 4},
    {0, 7, 5, 2, 1, 6, 4, 3}
};
//GF(2^3)生成矩阵
const vector<vector<int> >  GenerMatix = {
    {1, 0, 0},
    {0, 1, 0},
    {0, 0, 1},
    {6, 4, 3},
    {1, 1, 1},
    {6, 5, 2},
    {7, 5, 3}
};
//GF(2^3)的校验矩阵
const vector<vector<int> > HMatix = {
    {6, 4, 3, 1, 0, 0, 0},
    {1, 1, 1, 0, 1, 0, 0},
    {6, 5, 2, 0, 0, 1, 0},
    {7, 5, 3, 0, 0, 0, 1}
};
int b[3][3] = {{1, 0, 0},
              {0, 1, 0},
            {0, 0, 1}}; //最后得出的逆矩阵
int a[3][3] = {0};
const vector<int> before_M = {6, 5, 4};
const int size_n = 7;
const int size_r = 3;
vector<int> after_M(7);
int main()
{
    vector<vector<int> > GenerMatix_After = GenerMatix;
    for(int i = 0; i < size_n; i++){
        after_M[i] = 0;
        for(int j = 0; j < size_r; j++){
             after_M[i] = fun_addAndSubtract(after_M[i], fun_Multiply(GenerMatix[i][j], before_M[j]));
        }
    }
    cout << "编码后的数据为：" << endl;
    for(int i = 0; i < after_M.size(); i++){
        cout << after_M[i] << " ";
    }
    cout << endl;

    //产生100个100以内不重复随机数
    int temp[100] = {0};
    for(int i=0; i<=6; ++i) temp[i]=i;
    for(int i=6; i>=1; --i) swap(temp[i], temp[rand()%i]);
    cout << "随机删除元素前的结果为：" << endl;
    for(int i = 0; i < GenerMatix_After.size(); i++){
        for(int j = 0; j < GenerMatix_After[i].size(); j++){
            cout << GenerMatix_After[i][j] << " ";
        }
        cout << endl;
    }
    for(int i = 0; i < 4; i++){
        GenerMatix_After.erase(GenerMatix_After.begin() + temp[i]); //用前面生成的随机数 随机删除几行 模拟块的失效
        after_M.erase(after_M.begin() + temp[i]);
    }
    cout << "随机删除元素后，生成矩阵的结果为：" << endl;
    for(int i = 0; i < GenerMatix_After.size(); i++){
        for(int j = 0; j < GenerMatix_After[i].size(); j++){
            cout << GenerMatix_After[i][j] << " ";
        }
        cout << endl;
    }
    cout << "随机删除元素后，剩余的元素的结果为：" << endl;
    for(int i = 0; i < after_M.size(); i++){
        cout << after_M[i] << " ";
    }
    cout << endl;
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            a[i][j] = GenerMatix_After[i][j];
        }
    }
    if(fun_GaussBaseGf(a, 3)){
        cout << "经过高斯消元，生成的逆矩阵为：" << endl;;
        print(b, 3);
    }
    vector<int> res(3);
    for(int i = 0; i < 3; i++){
        res[i] = 0;
        for(int j = 0; j < 3; j++){
            res[i] = fun_addAndSubtract(res[i], fun_Multiply(b[i][j], after_M[j]));
        }
    }
    cout << "恢复出来的数据为：" << endl;
    int flag = 1;
    for(int i = 0; i < 3; i++){
        cout << res[i] << " ";
        if(res[i] != before_M[i]){
            flag = 0;
        }
    }
    if(flag){
        cout << "恢复成功！" << endl;
    }else {
        cout << "恢复失败！" << endl;
    }
    getchar();

}

//打印出矩阵
void print(int (*x)[3], int row_size)
{
    for(int i = 0; i < row_size; i++){
        for(int j = 0; j < 3; j++){
            cout << setw(10) << x[i][j] << " ";
        }
        cout << endl;
    }
}
//分别对应伽罗华域中的运算 加减--异或 乘为查表操作 除以反查表操作
int fun_addAndSubtract(int a, int b)
{
    return add_v[a][b];
}
int fun_Multiply(int a, int b)
{
    return multi_v[a][b];
}
int fun_divide(int a, int b)
{
    for(int k = 0; k < 8; k++){
        if(multi_v[b][k] == a){
            return k;
        }
    }
}
//高斯消元法
int fun_GaussBaseGf(int (*a)[3], int row_size)
{
    cout << "原始矩阵为：" << endl;
    print(a, 3);

    int i, j, k, N = 3;
    //查找主元
    for(i = 0; i < N; i++){
        int max = a[i][i];
        k = i;
        for(j = i + 1; j < N; j++){
            if(abs(a[j][i]) > abs(max)){
                max = a[j][i];
                k = j;
            }
        }
        //如果第i行不是主元，则将第i行与第k行进行互换(记得要连着单位矩阵)
        if(k != i){
            for(j = 0; j < N; j++){
                int temp = a[i][j];
                a[i][j] = a[k][j];
                a[k][j] = temp;

                temp = b[i][j];
                b[i][j] = b[k][j];
                b[k][j] = temp;
            }
            cout << "查找主元，经过变换后矩阵为：" << endl;
            print(a, N);
        }

        //判断主元是否为0，若是，则矩阵A不是满秩矩阵
        if(a[i][i] == 0){
            cout << "There is no inverse matrix!" << endl;
            return 0;
        }

        int temp = a[i][i]; //temp为要收缩的比例
        for(k = 0; k < N; k++){
            a[i][k] = fun_divide(a[i][k], temp);
            b[i][k] = fun_divide(b[i][k], temp);
        }

        for(j = 0; j < N; j++){
            if(j != i){
                int temp = a[j][i];
                for(k = 0; k < N; k++){
                    a[j][k] = fun_addAndSubtract(a[j][k], fun_Multiply(a[i][k], temp));
                    b[j][k] = fun_addAndSubtract(b[j][k], fun_Multiply(b[i][k], temp));
                }
            }
        }
        cout <<"第" << i + 1 << "列"<< "经过变换后矩阵为：" << endl;
        print(a, N);
        //getchar();
    }
    cout <<"最后变换后矩阵为：" << endl;
    print(b, N);
    return 1;
}
