#include <iostream>
#include <cstdio> 
#include <fstream>
#include <string>
#include <bitset> 
#include <iomanip>

using namespace std;

int judge(string ins){
	if (ins.substr(0,4)=="add " || ins.substr(0,4)=="sub " 
	|| ins.substr(0,4)=="and " || ins.substr(0,4)=="slt "
	) return 0;
	
	if (ins.substr(0,6)=="addiu " || ins.substr(0,5)=="andi " 
	|| ins.substr(0,4)=="ori " || ins.substr(0,5)=="xori " 
	|| ins.substr(0,5)=="slti " || ins.substr(0,4)=="beq " 
	|| ins.substr(0,4)=="bne "
	) return 1;	
	
	if (ins.substr(0,4)=="sll "
	) return 2;
	
	if (ins.substr(0,5)=="bltz "
	) return 3;
	
	if (ins.substr(0,2)=="j " || ins.substr(0,4)=="jal "
	) return 4;
	
	if (ins.substr(0,3)=="jr "
	) return 5;
	
	if (ins.substr(0,4)=="halt"
	) return 6;
	
	if (ins.substr(0,3)=="sw " || ins.substr(0,3)=="lw "
	) return 7;
}


string extend(string s,int num){
	string t(num-s.length(),'0');
	return t+s;
}

string trans_0(string ins){
	//0
	//add,sub,and,slt,
	//opcode(6) rs(5) rt(5) rd(5) reserved(11)
	//cout<<ins<<endl<<endl;
	
	int k=0;
	string ret,rs,rt,rd;
	if (ins.substr(0,3)=="add") ret="000000";
	if (ins.substr(0,3)=="sub") ret="000001";
	if (ins.substr(0,3)=="and") ret="010000";
	if (ins.substr(0,3)=="slt") ret="100111";
	
	int pos=ins.find('$');
	while (pos!=string::npos){
		string temp=ins.substr(pos+1,ins.find(',',pos+1)-pos-1);
		bitset<5> b(stoi(temp));
		if (k==0) rd=b.to_string();
		else if (k==1) rs=b.to_string();
		else rt=b.to_string();
		k++;
		pos=ins.find('$',pos+1);
	}
	ret=ret+rs+rt+rd+extend("0",11);
	return ret;
}

string trans_1(string ins){
	//1
	//addiu,andi,ori,xori,slti,sw,lw,beq,bne
	//opcode(6) rs(5) rt(5) immediate(16)
	
	int k=0;
	string ret,rs,rt;
	if (ins.substr(0,5)=="addiu") ret="000010";
	if (ins.substr(0,4)=="andi") ret="010001";
	if (ins.substr(0,3)=="ori") ret="010010";
	if (ins.substr(0,4)=="xori") ret="010011";
	if (ins.substr(0,4)=="slti") ret="100110";
	if (ins.substr(0,3)=="beq") ret="110100";
	if (ins.substr(0,3)=="bne") ret="110101";
	
	int pos=ins.find('$');
	while (pos!=string::npos){
		string temp=ins.substr(pos+1,ins.find(',',pos+1)-pos-1);
		bitset<5> b(stoi(temp));
		if (k==0) rt=b.to_string();
		else rs=b.to_string();
		k++;
		pos=ins.find('$',pos+1);
	}
	pos=ins.find(',');
	pos=ins.find(',',pos+1);
	bitset<16> b(stoi(ins.substr(pos+1)));
	ret=ret+rs+rt+b.to_string();
	return ret;
}

string trans_2(string ins){
	//2
	//sll rd,rt,sa
	//011000 Î´ÓÃ(5) rt(5) rd(5) sa(5) reserved(6)
	
	int k=0;
	string ret="01100000000",rt,rd,sa;
	
	int pos=ins.find('$');
	while (pos!=string::npos){
		string temp=ins.substr(pos+1,ins.find(',',pos+1)-pos-1);
		bitset<5> b(stoi(temp));
		if (k==0) rd=b.to_string();
		else rt=b.to_string();
		k++;
		pos=ins.find('$',pos+1);
	}
	pos=ins.find(',');
	pos=ins.find(',',pos+1);
	bitset<5> b(stoi(ins.substr(pos+1)));
	ret=ret+rt+rd+b.to_string()+"000000";
	return ret;
}

string trans_3(string ins){
	//3
	//bltz rs,immediate
	//110110 rs(5) 00000 immediate(16)
	
	string ret="110110",rs;
	
	int pos=ins.find('$');
	while (pos!=string::npos){
		string temp=ins.substr(pos+1,ins.find(',',pos+1)-pos-1);
		bitset<5> b(stoi(temp));
		rs=b.to_string();
		pos=ins.find('$',pos+1);
	}
	pos=ins.find(',');
	bitset<16> b(stoi(ins.substr(pos+1)));
	ret=ret+rs+"00000"+b.to_string();
	return ret;
}

string trans_4(string ins){
	//4
	//j jal
	//111000 addr[27:2]
	//111010 addr[27:2]
	
	string ret;
	if (ins.substr(0,3)=="jal") ret="111010";
	else ret="111000";
	int pos=ins.find(' ');
	bitset<32> b(stoi(ins.substr(pos+1),0,16));
	ret=ret+(b.to_string()).substr(4,26);
	return ret;
}

string trans_5(string ins){
	//5
	//jr rs
	//111001 rs(5) reserved(21)
	
	string ret="111001";
	int pos=ins.find('$');
	bitset<5> b(stoi(ins.substr(pos+1)));
	ret=ret+b.to_string()+extend("0",21);
	return ret;
}

string trans_6(string ins){
	//6
	//halt
	//111111 0(26)
	
	string ret="111111"+extend("0",26);
	return ret;
}

string trans_7(string ins){
	//7
	//lw,sw
	//lw rt,immediate(rs)
	//opcode(6) rs(5) rt(5) immediate(16)
	
	int k=0;
	string ret,rs,rt;
	if (ins.substr(0,2)=="sw") ret="110000";
	else ret="110001";
	
	int pos=ins.find('$');
	while (pos!=string::npos){
		string temp=ins.substr(pos+1,ins.find(',',pos+1)-pos-1);
		bitset<5> b(stoi(temp));
		if (k==0) rt=b.to_string();
		else rs=b.to_string();
		k++;
		pos=ins.find('$',pos+1);
	}
	pos=ins.find(',');
	bitset<16> b(stoi(ins.substr(pos+1,ins.find('(',pos+1)-pos-1)));
	ret=ret+rs+rt+b.to_string();
	return ret;
}


string translate(string ins){
	int type=judge(ins);
	string ret;
	switch (type){
		case 0:ret=trans_0(ins);break;
		case 1:ret=trans_1(ins);break;
		case 2:ret=trans_2(ins);break;
		case 3:ret=trans_3(ins);break;
		case 4:ret=trans_4(ins);break;
		case 5:ret=trans_5(ins);break;
		case 6:ret=trans_6(ins);break;
		case 7:ret=trans_7(ins);break;
	}
	/*ret.insert(6,1,' ');
	ret.insert(11+1,1,' ');
	ret.insert(16+2,1,' ');
	ret.insert(21+3,1,' ');*/
	
	ret.insert(8,1,' ');
	ret.insert(16+1,1,' ');
	ret.insert(24+2,1,' ');
	return ret;
} 


int main(){
	ifstream in("mips_ins.txt");
	//ifstream in("test.txt");
	ofstream out("ins.txt");
	ofstream out2("16_ins.txt");
	//ofstream out("test_ans.txt");
	string s,t;
	out2.flags(ios::hex | ios::uppercase);
	out2.fill('0');
	while (!in.eof() && getline(in,s)){
		t=translate(s);
		out<<t<<endl;
	//	bitset<32> b(t);
	//	out2<<setw(8)<<b.to_ulong()<<endl;
	}
	return 0;
}
