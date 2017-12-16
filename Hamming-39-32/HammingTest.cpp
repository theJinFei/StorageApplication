#include "HammingTest.h"


bitset<dataLen> enInput("00001100000000000000000000000000");
bitset<codeLen> enOutput;
bitset<codeLen> deInput;
bitset<codeLen> deOutput;
bitset<parityLen> parityXOR;
vector<bitset<dataLen> > testSecInput(4, bitset<dataLen> (0)); //输入 以四个编码条纹为一组 进行第二级的纠错
vector<bitset<codeLen> > testSecOutput(4, bitset<codeLen> (0)); //输出 以四个编码条纹为一组 进行第二级的纠错
vector<bitset<codeLen> > testSecDeInput(4, bitset<codeLen> (0)); //进行编码 输入 以四个编码条纹为一组 进行第二级的纠错
vector<bitset<codeLen> > testSecDeOutput(4, bitset<codeLen> (0)); //进行编码 输出 以四个编码条纹为一组 进行第二级的纠错
bitset<codeLen> testXOR;

//用matlab算正确的结果应该为 string temp = "00001100000000000000000000000000 0001111";

unordered_map<bitset<parityLen>, bitset<codeLen>> errTable;

//Hamming(39,32)生成矩阵 32 * 39的生成矩阵
vector<vector<int>> generateMatrix = {
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,1,0,1},
    {0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0},
    {0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,1,1,0,1},
    {0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,1,0,1,1},
    {0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,1,0,0,0},
    {0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,1},
    {0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,0,0},
    {0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,1,0},
    {0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0},
    {0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,1,0},
    {0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1},
    {0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,1,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,1},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,1},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,1},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,1,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,1},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,1,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,1,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,1},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,1,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,1},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,1,1},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,1,1,1,0,1},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,1,1,0,1,1},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,1,1,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,0,1,0,1,1,1},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,1,0,1,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,1,0,0,1,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,1,0,1,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,1,1}
};

//Hamming(39,32)校验矩阵 7 * 39的校验矩阵
//根据 H * A(39位信息码) = 0矩阵 以及相应的编码规则得出
int adjointMatrix_T[39][7]; //存放校验矩阵的转置
int adjointMatrix[7][39] = {
    {
        1, 1, 1, 1, 1, 1, 1, 1,  //31-24
        0, 0, 0, 0, 0,  //23-19
        0, 0, 0, 0, 0,  //18-14
        0, 0, 0, 0, 0, 0,   //13-8
        1, 1, 1, 1, 1, 1, 1, 1, //7-0
        1, 0, 0, 0, 0, 0, 0
    }, //单位矩阵

    {
        1, 1, 1, 1, 1, 1, 1, 1,  //31-24
        0, 0, 0, 0, 0,  //23-19
        0, 0, 0, 1, 1,  //18-14
        1, 1, 1, 1, 1, 1,   //13-8
        0, 0, 0, 0, 0, 0, 0, 0, //7-0
        0, 1, 0, 0, 0, 0, 0
    }, //单位矩阵

    {
        1, 1, 0, 0, 0, 0, 0, 0,  //31-24
        1, 1, 1, 1, 1,  //23-19
        1, 0, 0, 1, 1,  //18-14
        0, 0, 0, 0, 0, 0,   //13-8
        1, 1, 1, 1, 1, 1, 0, 0, //7-0
        0, 0, 1, 0, 0, 0, 0
    }, //单位矩阵

    {
        0, 0, 1, 1, 1, 0, 0, 0,  //31-24
        1, 1, 1, 0, 0,  //23-19
        0, 1, 1, 0, 0,  //18-14
        1, 1, 1, 0, 0, 0,   //13-8
        1, 1, 1, 0, 0, 0, 1, 1, //7-0
        0, 0, 0, 1, 0, 0, 0
    }, //单位矩阵

    {
        1, 0, 1, 0, 0, 1, 1, 0,  //31-24
        1, 0, 0, 1, 1,  //23-19
        0, 0, 1, 1, 0,  //18-14
        1, 0, 0, 1, 1, 0,   //13-8
        1, 0, 0, 1, 1, 0, 0, 1, //7-0
        0, 0, 0, 0, 1, 0, 0
    }, //单位矩阵

    {
        0, 0, 0, 1, 0, 1, 0, 1,  //31-24
        0, 1, 0, 1, 0,  //23-19
        1, 1, 1, 0, 0,  //18-14
        0, 1, 0, 1, 0, 1,   //13-8
        0, 1, 0, 1, 0, 1, 1, 1, //7-0
        0, 0, 0, 0, 0, 1, 0
    }, //单位矩阵

    {
        1, 0, 1, 1, 0, 1, 0, 0,  //31-24
        0, 0, 1, 0, 1,  //23-19
        1, 1, 0, 0, 1,  //18-14
        0, 0, 1, 0, 1, 1,   //13-8
        1, 1, 0, 1, 0, 0, 0, 1, //7-0
        0, 0, 0, 0, 0, 0, 1
    } //单位矩阵
};



/**
Hamming编码
@input 输入32位信息位
@output 输出39位编码后的Hamming码
*/
void HammingTest::HammingEncode(bitset<dataLen> &input, bitset<codeLen> &output) {
    //printf("\tHammingEncode is running...\n");
    //cout << "\tinput is  " << input.to_string() << endl;
    //信息位乘以生成矩阵时 要做一点变换 把相应的下标搞懂就可以了
    for(int j = 0; j < codeLen; j++) {
        for(int i = 0; i < dataLen; i++) {
            if(input[31 - i] * generateMatrix[i][j]) {
                output.flip(38 - j);
            }
        }
    }
    //cout<<"\toutput is " << output.to_string() << endl;
    //printf("\tHammingEncode is ending...\n");
}

/**
Hamming解编码
@input 收到的39位码组
@output 看是否有错误
*/
void HammingTest::HammingDecode(bitset<codeLen> &input, bitset<codeLen> &output) {
    //printf("\tHammingDecode is running...\n");
    getParityXOR(input, parityXOR);
    //cout << "\tparityXOR is " << parityXOR << endl;
    if(!parityXOR.any()) {
        output = input;
        return;
    }
    //如果errTable中有该错误模式，则修复
    auto existTable = errTable.find(parityXOR);
    if(existTable != errTable.end()) {
        output = input ^ errTable[parityXOR];
    } else {
        output = input;
        //cout << "错误过多，无法修复" << endl;
    }
    //printf("\tHammingDecode is ending...\n");
}

/**
求错误的伴随式
@input 接收到的数据
@output 错误伴随式
*/
void HammingTest::getParityXOR(bitset<codeLen> &input, bitset<parityLen> &output) {
    //printf("getParityXOR is running...\n");
    //求伴随矩阵的转置
    for (int i = 0; i < 39; i++) {
        for (int j = 0; j < 7; j++) {
            adjointMatrix_T[i][j] = adjointMatrix[j][i];
        }
    }
    for(int i  = 0; i < 7; i++) {
        output[i] = 0;
        for(int j = 0; j < 39; j++) {
            if(input[38 - j] * adjointMatrix[i][j]) {
                output.flip(i);
            }
        }
    }
    //cout << output.to_string() << endl;
    //printf("getParityXOR is ending...\n");
}

/**
制表，保存所有的1位错误产生的7位异或值
*/
void HammingTest::setTable() {
    //生成文件流
    string fileName = "errTableHamming_39_32.txt";
    ofstream fout(fileName, ios::binary);
    fout << "Loc\t" << "XOR\t" << "\n";

    //编码
    //cout << enInput << endl;
    //先指定一个信息码 附上相应的校验位然后通过指定相应的位出错
    //通过打一个表 之后就可以直接查询该表
    HammingEncode(enInput, enOutput);
    //cout << enOutput << endl;

    //出现1位错误，错误位置在errLoc上
    for(int errLoc = 0; errLoc < codeLen; errLoc++) {
        deInput = enOutput;
        deInput.flip(errLoc);

        getParityXOR(deInput, parityXOR);
        bitset<codeLen> tmp;
        tmp.flip(errLoc);
        fout << tmp << "\t" << parityXOR << "\t\n";
    }
}

/**
将表读到内存中去，映射到errTable中，下标为伴随式，值为错误位置
*/
void HammingTest::getTable() {
    //生成文件流
    string fileName = "errTableHamming_39_32.txt";
    ifstream fin(fileName, ios::binary);
    string tmpStr;
    bitset<codeLen> temp1;
    bitset<parityLen> temp2;
    fin >> tmpStr >> tmpStr; //去表头
    while(!fin.eof()) {
        fin >> temp1 >> temp2;
        errTable[temp2] = temp1;
    }
}

/**
Hamming测试
*/
void HammingTest::testHamming() {
    cout << "$1:\tsetTable(制作错误位置表，保存到errTableHamming_39_32.txt)" << endl;
    setTable();
    cout << "$2:\tgetTable(从errTableHamming_39_32.txt读去错误模式，并倒排保存到errTable数组中，下标为伴随式，值为错误位置)" << endl;
    getTable();

    cout << "$3:\t生成一组随机输入序列：\t\n";
    srand((unsigned int)time(NULL));
    int tmp = 0;
    enInput.reset();
    //cout << testSecInput.size() << " " << testSecInput[0].size() <<endl;
    for(int j = 0; j < testSecInput.size(); j++) { //生成4个32位的随机序列
        for (int i = 0; i < testSecInput[j].size(); i ++) {
            tmp = rand() % 2;
            if (tmp) {
                //enInput.set(i);
                testSecInput[j].set(i);
            }
        }
        //cout << "\t海明编码前的码字：\t";
        //cout << testSecInput[j] << endl;
        HammingEncode(testSecInput[j], testSecOutput[j]);
        cout << "\t海明编码后的码字：\t";
        cout << testSecOutput[j] << endl;

    }
    //实行偶校验
    for (int i = 0; i < blocksize; i ++) {
        int temp = 0;
        for(int j = 0; j < testSecOutput.size(); j++) {
            if(testSecOutput[j][i]) {
                temp++;
            }
        }
        if(temp % 2 == 0) {
            testXOR[i] = 0;
        } else {
            testXOR[i] = 1;
        }
    }
    cout << "\t奇偶校验的结果为：\t";
    cout << testXOR << endl;
    bool isEnd = 0;
    int errNum = 0;
    while(!isEnd) {
        if(isEnd) {
            break;
        }
        testSecDeInput = testSecOutput;
        cout << "\t请输入错误序号，（0-3）范围，以-1结束:  \t";
        while(cin >> errNum) {
            if(errNum == -1) {
                break;
            }
            cout << "\t请输入错误位置(0-38),低位在后，高位在前: \t";
            vector<int> errLoc;
            int tempLoc = 0;
            while(cin >> tempLoc) {
                errLoc.push_back(tempLoc);
                if(cin.get() == '\n') {
                    break;
                }
            }
            //testSecDeInput[errNum] = testSecOutput[errNum];
            for(int i = 0; i < errLoc.size(); i++) {
                testSecDeInput[errNum].flip(errLoc[i]);
            }
            cout << "\t插入错误后的结果为: \t";
            cout << testSecDeInput[errNum] << endl;
            cout << "\t请继续输入错误序号，（0-3）范围，以-1结束: \t";
        }
        //先执行这个循环，将只发生1位错误编码的情况 给纠错出来
        for(int i = 0; i < testSecDeInput.size(); i++) {
            HammingDecode(testSecDeInput[i], testSecDeOutput[i]);
        }
//        cout << " testSecDeOutput \t" << testSecDeOutput[i] << endl;
//        cout << " testSecOutput   \t" << testSecOutput[i] << endl;
        for(int i = 0; i < testSecDeOutput.size(); i++) {
            if(testSecDeOutput[i] == testSecOutput[i]) {
                cout << "\t利用字内编码机制修复成功,修复后的序列为： \t" << testSecDeOutput[i] <<endl;
            } else {
                //如果只有1个序列其中的2位以上发生了错误，则抛给异或进行解码
                cout << "\t这个序列为：\t\t" << testSecDeOutput[i] << "\t发生了2位以上的错误，进入第二级冗余校验" << endl;
                //做异或校验
                for (int j = 0; j < blocksize; j ++) {
                    int temp = 0;
                    for(int k = 0; k < testSecDeOutput.size(); k++) {
                        //cout << "\ttestSecOutput:\t" << testSecOutput[k] << endl;
                        if(k != i && testSecDeOutput[k][j]) {
                            temp++;
                            //cout << "\tk:\t" << j << "\tj:\t" << j << testSecDeOutput[k][j] << endl;
                        } else {
                            continue;
                        }
                    }
                    if(testXOR[j]) {
                        temp++;
                    }
                    //cout << " temp " << temp << endl;
                    if(temp % 2 != 0) {
                        testSecDeOutput[i][j] = 1;
                    } else {
                        testSecDeOutput[i][j] = 0;
                    }
                }
                //cout << "2 testSecDeOutput   " << testSecDeOutput[i] << endl;
                //cout << "2 testSecOutput   " << testSecOutput[i] << endl;
                if(testSecDeOutput[i] == testSecOutput[i]) {
                    cout << "\t利用字间编码机制修复成功,修复后的序列为： \t" << testSecDeOutput[i] <<endl;
                } else {
                    cout << "\t存在两个序列以上发生2位以上的错误，修复失败，请选用其他纠错码。\t" << endl;
                }
            }
        }
        cout << "\t退出测试？ yes : 1 , no : 0\t";
        cin >> isEnd;
    }



//    cout << enInput << endl;
//    enOutput.reset();
//    HammingEncode(enInput, enOutput);
//    cout << "\t海明编码后的码字：\t";
//    cout << enOutput << endl;
//    bool notEnd = 1;
//    while(notEnd){
//        cout << "\t请输入错误位置(0-38),低位在后，高位在前" << endl;
//        int errLoc = 0;
//        cin >> errLoc;
//        deInput = enOutput;
//        deInput.flip(errLoc);
//        cout << "\t注入错误后的接收信息序列为" << deInput << endl;
//        HammingDecode(deInput, deOutput);
//        if(deOutput == enOutput){
//            cout << "\t修复成功" << endl;
//            cout << "\t修复错误后的接收信息序列为" << deOutput << endl;
//        }else{
//            cout << "\t修复失败" << endl;
//        }
//    }
}


