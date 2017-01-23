#include <iostream>
#include <string>
#include <vector>

using namespace std;

struct token{
	string type;
	float number;
};

struct tokens{
	token cont;
	int index;
};

float calcu(string line);

token readNumber(string line,int &index)
{
	token ret;
	float number = 0;
	int flag = 0;		//小数点がでてきたら旗を上げる
	float keta = 0.1;
	//数字ではなくなるまで読む
	while(index < line.length())
	{
		if(isdigit(line[index]) || line[index] == '.'){
		//読み込んだものが小数点だったら旗を立てる
			if(line[index] == '.'){
				flag = 1;
			}else if(!flag){
				number = number * 10 + (line[index] - '0');
			}else{
				number += (line[index] - '0') * keta;
				keta *= 0.1;
			}
		}else{
			break;
		}
		index++;
	}
	ret.type = "NUMBER";
	ret.number = number;
	return ret;
} 

void Plus(token &ret){
	ret.type = "PLUS";
}

void Minus(token &ret,string &line,const int &index){
	if (line[index+1] == '-'){
			ret.type = "PLUS";
			line[index+1] = '+';
	}else{
		ret.type = "MINUS";
	}
}

token read_PlusAndMinus(string line,int &index,const string &mark)
{
	token ret;
	if(mark == "MINUS"){	//MINUSだった場合
		Minus(ret,line,index);
	}else{					//PLUSだった場合
		Plus(ret);
	}
	index++;
	return ret;
}

token evaluateMultiplyAndDivide(string line,int &index,vector<tokens> &tokens0,const string &mark)
{
	token ret,next;
	tokens last;
	ret.type = "NUMBER";
	index++;
	last = tokens0.back();		//一つ前の要素
	if (last.cont.type != "NUMBER")	//記号が二つ続いてたら
	{
		cout << "symbol error" << endl;
	}
	next = readNumber(line,index);
	if (mark == "MULTI"){
		ret.number = last.cont.number * next.number;
	}else if (mark == "DIVIDE"){
		ret.number = last.cont.number / next.number;
	}
	tokens0.pop_back();
	return ret;
}

void tokenize(string line,vector<tokens> &tokens0)
{
	int index = 0;
	int index2 = line.length()-1;
	tokens tokens1;
	//tokenにわけてtokensにいれる
	while(index < line.length()){
		if (isdigit(line[index])){
			tokens1.cont = readNumber(line,index);
		}else if (line[index] == '+'){
			tokens1.cont = read_PlusAndMinus(line,index,"PLUS");
		}else if (line[index] == '-'){
			tokens1.cont = read_PlusAndMinus(line,index,"MINUS");
		}else if(line[index] == '*' ){
			tokens1.cont = evaluateMultiplyAndDivide(line,index,tokens0,"MULTI");
		}else if (line[index] == '/'){
			tokens1.cont = evaluateMultiplyAndDivide(line,index,tokens0,"DIVIDE");
		}else{
			cout << "Invalid character found: " << line[index] << endl;
			cout << line[index] << endl;
			exit(1);
		}
		tokens1.index = index;
		//配列に加える！
		tokens0.push_back(tokens1);
	}
}

float evaluatePlusAndMinus(vector<tokens> &tokens1)
{
	float answer = 0;
	//配列の最初に+をいれる！
	tokens tokens0;
	tokens0.cont.type = "PLUS";		//最初は必ず+!!
	tokens1.insert(tokens1.begin(),tokens0);
	//配列[0]には上で値を入れたからindexは1から
	int index = 1;

	while(index < tokens1.size()){
		if(tokens1[index].cont.type == "NUMBER"){
			if (tokens1[index-1].cont.type == "PLUS"){
				answer += tokens1[index].cont.number;
			}else if (tokens1[index-1].cont.type == "MINUS"){
				answer -= tokens1[index].cont.number;
			}else{

				cout << "Invalid syntax" << endl;
			}
		}
		index++;
	}
	return answer;
}

void evaluateBracket(string &line){
	int index =0;
	int index2 = 0;
	int flag =0;
	float newline;

	for (;index<line.length();index++){
		if(line[index] == '('){
			for (int i=0; i <=line.length() ; i++){
				if(line[i] == '('){		//check () in ()
					index2++;
				}
				else if(line[i] == ')'){
					index2--;
					if (index2==0){
					newline = calcu(line.substr(index+1,i-index-1));
					line.replace(index,i-index+1,to_string(newline));
					break;
					}
				}
			}
		}
	}
}

float calcu(string line){
	std::vector<tokens> tokens0;
	float answer;

	evaluateBracket(line);		//先に()計算
	tokenize(line,tokens0);		//区切る
	answer = evaluatePlusAndMinus(tokens0);
	return answer;
}

int main(int argc, char const *argv[])
{
	string line = "";
	float answer = 0;

	cout << ">";
	cin >> line;
	answer = calcu(line);
	cout << "ansewr = " << answer << endl;
	return 0;
}