
#include"reviewer_assigner.h"

int main(){
  ReviewerAssigner today(50);
  today.load();
  today.choose();
  today.output();
}
