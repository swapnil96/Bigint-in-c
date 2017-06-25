extern FILE* yyin;
extern FILE* out;
extern int yylineno; /* from lexer */
extern int max;


typedef struct node
{
    int digit;
    struct node* next;
    struct node* prev;
}node;

typedef struct number
{
    struct node* decimal_head;
    struct node* decimal_tail;
    struct node* fractional_head;
    struct node* fractional_tail;
	int decimal_length;
    int fractional_length;
    int sign;
}number;

char* convert_to_string(number a);

void insert_end(int data, node* tail);

void insert_front(int data, node* head);

void print_list(node *head);

void print_number(number* a);

void initialize(number* num, int decimal, int length);

number* convert_to_linked_list(char *a);

void insert_zeroes_front(node* x, int count);

void insert_zeroes_end(node* x, int count);

number* copy(number a);

void same(number* a, number* b);

int compare(number c, number d);

void remove_trailing_zeroes(number* a);

void remove_leading_zeroes(number* a);

number* addition(number c, number d);

number* subtraction(number c, number d);

number* temp_multiply(number* a , int n);

void change_suitable_multiplication (number *a);

void change_suitable_ans(number *a,int len);

number* multiplication (number c, number d);

int quotient_help(number* dividend, number* divisor);

void change_suitable_division(number* a,int len);

number* remainder_help(number* a, number* b, int quo);

number* quotient(number c, number d);

number* modulo(number a, number b);

number* division(number c, number d);

number* form_num(int temp);

number* form_one();

number* form_zero();

number* square_root(number c);

number* power(number c,number d);

number* logarithm(number a);

number* second_power(number c, number d);

number* final_addition(number c, number d);

number* final_subtraction(number c, number d);

number* final_multiplication(number c, number d);

number* final_division(number c, number d);

int check_fraction(number a);

number* exponential(number a);

void changePrec(number *a);
