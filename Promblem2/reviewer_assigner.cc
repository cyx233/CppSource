#include<fstream>
#include<iostream>
#include<cstring>
#include<stdlib.h>
#include"reviewer_assigner.h"

ReviewerAssigner::ReviewerAssigner(int num){
  student_list = new Student[num];
  tot_num = 0;
  memset(lucky_num,0,sizeof(lucky_num));
}

ReviewerAssigner::~ReviewerAssigner(){
    delete[] student_list;
}

bool ReviewerAssigner::load(){
  std::ifstream myfile;
  myfile.open("Promblem2/ContactEmail.txt");
  int i=0;
  if (myfile.is_open()){
    while (myfile.eof()){
      myfile>>student_list[i].num;
      myfile>>student_list[i].email;
      tot_num++;
      i++;
    }
    myfile.close();
    return true;
  }
  else
    return false;
}

void ReviewerAssigner::choose(){
  srand((unsigned)time(NULL)); 
  for(int i=0;i<=2;i++)
    lucky_num[i] = rand();
  return;
}

void ReviewerAssigner::output(){
  std::ofstream myfile;
  myfile.open("Problem2/reviewer.txt");
  if(myfile.is_open()){ 
    int n = 0;
    for(int i=0;i<=2;i++){
      n = lucky_num[i];
      std::cout<<student_list[i].num<<" "<<student_list[i].email<<std::endl;
    }
    myfile.close();
  }
  return;
}