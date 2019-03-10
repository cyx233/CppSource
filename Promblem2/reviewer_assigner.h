class ReviewerAssigner
{
 private:
  struct Student
  {
    int num;
    char email[50];
  };
  Student *student_list;
  int lucky_num[3];
  int tot_num;

 public:
  ReviewerAssigner(int num);
  ~ReviewerAssigner();
  bool load();
  void choose();
  void output();
};
