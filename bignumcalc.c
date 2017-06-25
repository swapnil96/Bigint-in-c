
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bignumcalc.h"

int max;
FILE* out;
void insert_end(int data, node* tail)
{

   		struct node *link = (struct node*) malloc(sizeof(struct node));
   		link->digit = data;
   		link->next = tail;
		link->prev = tail->prev;
		tail->prev->next = link;
		tail->prev = link;
}

void insert_front(int data, node* head)
{
	struct node *link = (struct node*) malloc(sizeof(struct node));
   	link->digit = data;
	link->next = head->next;
	head->next->prev = link;
	head->next = link;
	link->prev = head;
}

void print_list(node *head)
{
	node *ptr = head->next;
    while(ptr->next != NULL) {
      fprintf(out, "%d", ptr->digit);
      ptr = ptr->next;
   }
}

void print_number(number* a)
{
	if(a == NULL){
		return;
	}
	if(a->sign == 1){
		fprintf(out, "%s", "- ");
	}
    print_list(a->decimal_head);
    if (a->fractional_length == 0 || (a->fractional_length == 1 && a->fractional_head->next->digit == 0))
    {
        return ;
    }
    fprintf(out, "%c", '.');
    print_list(a->fractional_head);
    fprintf(out, "%s", "\n");
}


void initialize(number* num, int decimal, int length)
{
	 num -> decimal_length = decimal;
    num -> fractional_length = length - decimal - 1;
	num -> decimal_head = (struct node*) malloc(sizeof(struct node));;
    num -> fractional_head = (struct node*) malloc(sizeof(struct node));;
    num -> decimal_tail = (struct node*) malloc(sizeof(struct node));;
    num -> fractional_tail = (struct node*) malloc(sizeof(struct node));;
	num->decimal_head->next = num->decimal_tail;
	num->decimal_tail->prev = num->decimal_head;
	num->decimal_head->prev = NULL;
	num->decimal_tail->next = NULL;
	num->fractional_head->next = num->fractional_tail;
	num->fractional_tail->prev = num->fractional_head;
	num->fractional_head->prev = NULL;
	num->fractional_tail->next = NULL;
}

struct number* convert_to_linked_list(char *a)
{
	 struct number* num = (struct number*) malloc(sizeof(struct number));
	int check = 0;
	int position_of_decimal = 0;
	if(*a == '-'){
		num->sign = 1;
		a = a+1;
	}
	else{
		num->sign = 0;
	}
	int i = 0;
		for( i = 0; i < strlen(a); i++)
    {
    		if ( *(a+i) == '.'){
        		position_of_decimal = i;
				check = 1;
            break;
        }
    }
    if(check == 0){
		position_of_decimal = strlen(a);
	}
	initialize(num, position_of_decimal, strlen(a));
    for(i = 0 ; i <=  position_of_decimal - 1; i++){
    	int no = *(a + i) - '0';
        insert_end(no, num->decimal_tail);
    }
	if(check == 0){
		insert_end(0,num->fractional_tail);
		num->fractional_length = 1;
	}
	else{
		num->fractional_length = 0;
		for(i = position_of_decimal + 1; i < strlen(a); i++){
			int no = *(a + i) - '0';
			insert_end(no, num->fractional_tail);
			num->fractional_length++;
		}
	}
	return num;
}

void insert_zeroes_front(node* x, int count)
{
    for (size_t i = 0; i < count; i++)
    {
        insert_front(0, x);
    }
}

void insert_zeroes_end(node* x, int count)
{
    for (size_t i = 0; i < count; i++) {
        insert_end(0, x);
    }
}

number* copy(number a)
{
    number* final = (struct number*) malloc(sizeof(struct number));
    initialize(final, 0, 0);
	final->sign = 0;
    node* itr1 = a.decimal_head->next;
    while (itr1->next != NULL)
    {
        insert_end(itr1->digit, final->decimal_tail);
        itr1 = itr1->next;
    }
    itr1 = a.fractional_head->next;
    while (itr1->next != NULL)
    {
        insert_end(itr1->digit, final->fractional_tail);
        itr1 = itr1->next;
    }
    final->fractional_length = a.fractional_length;
    final->decimal_length = a.decimal_length;
    return final;
}

void same(number* a, number* b)
{
    int offset = a->decimal_length - b->decimal_length;
	if (offset > 0)
    {
        insert_zeroes_front(b->decimal_head, offset);
        b->decimal_length = a->decimal_length;
    }
	else
    {
        insert_zeroes_front(a->decimal_head, -1*offset);
        a->decimal_length = b->decimal_length;
    }
    offset = a->fractional_length - b->fractional_length;
	if (offset > 0)
    {
        insert_zeroes_end(b->fractional_tail, offset);
        b->fractional_length = a->fractional_length;
    }
	else
    {
        insert_zeroes_end(a->fractional_tail, -1*offset);
        a->fractional_length = b->fractional_length;
    }

}

int compare(number c, number d)
{
    number* a = copy(c);
    number* b = copy(d);
	same(a, b);
    node* itr1 = a->decimal_head->next;
    node* itr2 = b->decimal_head->next;
    for (size_t i = 0; i < a->decimal_length; i++)
    {
        if (itr1->digit > itr2->digit)
            return 1;

        else if (itr1->digit < itr2->digit)
            return -1;

        itr1 = itr1->next;
        itr2 = itr2->next;
    }
    itr1 = a->fractional_head->next;
    itr2 = b->fractional_head->next;
    for (size_t i = 0; i < a->fractional_length; i++)
    {
        if (itr1->digit > itr2->digit)
            return 1;

        else if (itr1->digit < itr2->digit)
            return -1;

        itr1 = itr1->next;
        itr2 = itr2->next;
    }
    return 0;
}

void changePrec(number *a){
	int i = max - a->decimal_length;
	int j = 0;
	node* curr = a->fractional_head;
	while(j<i && curr->next != NULL){
		curr = curr->next;
		j++;
	}
	if(curr->next == NULL){
		return;
	}
	curr->next = a->fractional_tail;
	a->fractional_tail->prev = curr;
	a->fractional_length = i;
}
void remove_trailing_zeroes(number* a)
{
    if (a->fractional_length == 1)
    {
        return;
    }
	node* curr = a->fractional_head->next;
	a->fractional_length = 1;
	int i = 0;
	while( i < 20 && curr->next->next != NULL ){
		curr = curr->next;
		a->fractional_length ++;
		i++;
	}
	if(curr->next->next != NULL){
		curr->next = a->fractional_tail;
		a->fractional_tail->prev = curr;
	}
	while(a->fractional_tail->prev->prev != a->fractional_head){
		if(a->fractional_tail->prev->digit != 0){
			break;
		}
		else{
			a->fractional_tail->prev = a->fractional_tail->prev->prev;
			a->fractional_tail->prev->next = a->fractional_tail;
			a->fractional_length--;
		}
	}
}

void remove_leading_zeroes(number* a)
{
    if (a->decimal_length < 2)
    {
        return;
    }
	while(a->decimal_head->next->next->next != NULL){
		if(a->decimal_head->next->digit != 0){
			break;
		}
		a->decimal_head->next = a->decimal_head->next->next;
		a->decimal_head->next->prev = a->decimal_head;
		a->decimal_length--;
	}
}

number* final_addition(number c, number d)
{

    number* a = copy(c);
    number* b = copy(d);
	if(c.sign == 1 && d.sign == 0){
		number* ans1 = final_subtraction(*a,*b);
		if(compare(*a,*b) > 0){
			ans1->sign = 1;
		}
		else{
			ans1->sign = 0;
		}
		return ans1;
	}
	else if(c.sign == 0 && d.sign == 1){
		number* ans1 = final_subtraction(*a,*b);
		if(compare(*a,*b) >= 0){
			ans1->sign = 0;
		}
		else{
			ans1->sign = 1;
		}
		return ans1;
	}
	else if(c.sign == 1 && d.sign == 1){
		number* ans1 = final_addition(*a,*b);
		ans1->sign = 1;
		return ans1;
	}

	int offset;
    number* final = (struct number*) malloc(sizeof(struct number));
    initialize(final, 0, 0);
    same(a, b);
    node* itr1 = a->fractional_tail ;
    node* itr2 = b->fractional_tail;
    itr1 = itr1->prev;
    itr2 = itr2->prev;
    int carry = 0;
    for (int i = 0; i < a->fractional_length; i++)
    {
        int sum = itr1 -> digit;
        sum += itr2 -> digit + carry;
        if (sum > 9)
    	{
            carry = sum / 10;
            sum = sum % 10;
        }
        else
            carry = 0;

        insert_front(sum, final->fractional_head);
        if (final->fractional_length == -1)
        {
            final->fractional_length = 1;
        }
        else
        {
            final->fractional_length++;
        }
        itr1 = itr1->prev;
        itr2 = itr2->prev;
    }
    itr1 = a->decimal_tail;
    itr2 = b->decimal_tail;
    itr1 = itr1->prev;
    itr2 = itr2->prev;
    for (int i = 0; i < a->decimal_length; i++)
    {
        int sum = itr1 -> digit;
        sum += itr2 -> digit + carry;
        if (sum > 9)
    	{
            carry = sum / 10;
            sum = sum % 10;
        }
        else
            carry = 0;

        insert_front(sum, final->decimal_head);
        final->decimal_length++;
        itr1 = itr1->prev;
        itr2 = itr2->prev;
    }
    if (carry != 0)
    {
        insert_front(carry, final->decimal_head);
        final->decimal_length++;
    }

	remove_trailing_zeroes(final);
	if(final->decimal_length > max){
		fprintf(out, "%s", "LowPrec");
		return NULL;
	}
	changePrec(final);
    return final;
}
number* addition(number c, number d)
{
    number* a = copy(c);
    number* b = copy(d);
	int offset;
    number* final = (struct number*) malloc(sizeof(struct number));
    initialize(final, 0, 0);
    same(a, b);
    node* itr1 = a->fractional_tail ;
    node* itr2 = b->fractional_tail;
    itr1 = itr1->prev;
    itr2 = itr2->prev;
    int carry = 0;
    for (int i = 0; i < a->fractional_length; i++)
    {
        int sum = itr1 -> digit;
        sum += itr2 -> digit + carry;
        if (sum > 9)
    	{
            carry = sum / 10;
            sum = sum % 10;
        }
        else
            carry = 0;

        insert_front(sum, final->fractional_head);
        if (final->fractional_length == -1)
        {
            final->fractional_length = 1;
        }
        else
        {
            final->fractional_length++;
        }
        itr1 = itr1->prev;
        itr2 = itr2->prev;
    }
    itr1 = a->decimal_tail;
    itr2 = b->decimal_tail;
    itr1 = itr1->prev;
    itr2 = itr2->prev;
    for (int i = 0; i < a->decimal_length; i++)
    {
        int sum = itr1 -> digit;
        sum += itr2 -> digit + carry;
        if (sum > 9)
    	{
            carry = sum / 10;
            sum = sum % 10;
        }
        else
            carry = 0;

        insert_front(sum, final->decimal_head);
        final->decimal_length++;
        itr1 = itr1->prev;
        itr2 = itr2->prev;
    }
    if (carry != 0)
    {
        insert_front(carry, final->decimal_head);
        final->decimal_length++;
    }
	remove_trailing_zeroes(final);
	return final;
}

number* subtraction(number c, number d)
{
    number* final = (struct number*) malloc(sizeof(struct number));
    initialize(final, 0, 0);
	final->fractional_length = 0;
    number* x;
    number* y;
    int flag = compare(c, d);
    if (flag > 0)
    {
        x = copy(c);
        y = copy(d);
    }
    else
    {
        x = copy(d);
        y = copy(c);
    }
    same(x, y);
    node* itr1 = x->fractional_tail->prev;
    node* itr2 = y->fractional_tail->prev;
    int carry = 0;
    for (int i = 0; i < x->fractional_length; i++)
    {
        int temp1 = itr1->digit;
        int temp2 = itr2->digit;
        if (temp1 + carry >= temp2)
        {
            int sub = temp1 + carry - temp2;
            insert_front(sub, final->fractional_head);
            carry = 0;
        }
        else
        {
            int sub = temp1 + 10 - temp2 + carry;
            insert_front(sub, final->fractional_head);
            carry = -1;
        }
        final->fractional_length++;
        itr1 = itr1->prev;
        itr2 = itr2->prev;
    }

    itr1 = x->decimal_tail->prev;
    itr2 = y->decimal_tail->prev;
    for (int i = 0; i < x->decimal_length; i++)
    {
        int temp1 = itr1->digit;
        int temp2 = itr2->digit;
        if (temp1 + carry >= temp2)
        {
            int sub = temp1 + carry - temp2;
            insert_front(sub, final->decimal_head);
            carry = 0;
        }
        else
        {
            int sub = temp1 + 10 - temp2 + carry;
            insert_front(sub, final->decimal_head);
            carry = -1;
        }
        final->decimal_length += 1;
        itr1 = itr1->prev;
        itr2 = itr2->prev;
    }
	remove_trailing_zeroes(final);
    return final;
}

number* final_subtraction(number c, number d)
{
    number* final = (struct number*) malloc(sizeof(struct number));
    initialize(final, 0, 0);
	final->fractional_length = 0;
    number* x;
    number* y;
    int flag = compare(c, d);
    if (flag > 0)
    {
        x = copy(c);
        y = copy(d);
    }
    else
    {
        x = copy(d);
        y = copy(c);
    }
	if(c.sign == 1 && d.sign == 0){
		number* ans1 = final_addition(*x,*y);
		ans1->sign = 1;
		return ans1;
	}
	else if(c.sign == 0 && d.sign == 1){
		number* ans1 = final_addition(*x,*y);
		ans1->sign = 0;
		return ans1;
	}
	else if(c.sign == 1 && d.sign == 1){
		if(compare(c,d) <= 0){
			final->sign = 0;
		}
		else{
			final->sign = 1;
		}
	}
	else if(c.sign == 0 && d.sign == 0){
		if(compare(c,d) >= 0){
			final->sign = 0;
		}
		else{
			final->sign = 1;
		}
	}
    same(x, y);
    node* itr1 = x->fractional_tail->prev;
    node* itr2 = y->fractional_tail->prev;
    int carry = 0;
    for (int i = 0; i < x->fractional_length; i++)
    {
        int temp1 = itr1->digit;
        int temp2 = itr2->digit;
        if (temp1 + carry >= temp2)
        {
            int sub = temp1 + carry - temp2;
            insert_front(sub, final->fractional_head);
            carry = 0;
        }
        else
        {
            int sub = temp1 + 10 - temp2 + carry;
            insert_front(sub, final->fractional_head);
            carry = -1;
        }
        final->fractional_length++;
        itr1 = itr1->prev;
        itr2 = itr2->prev;
    }

    itr1 = x->decimal_tail->prev;
    itr2 = y->decimal_tail->prev;
    for (int i = 0; i < x->decimal_length; i++)
    {
        int temp1 = itr1->digit;
        int temp2 = itr2->digit;
        if (temp1 + carry >= temp2)
        {
            int sub = temp1 + carry - temp2;
            insert_front(sub, final->decimal_head);
            carry = 0;
        }
        else
        {
            int sub = temp1 + 10 - temp2 + carry;
            insert_front(sub, final->decimal_head);
            carry = -1;
        }
        final->decimal_length += 1;
        itr1 = itr1->prev;
        itr2 = itr2->prev;
    }
	remove_trailing_zeroes(final);
	if(final->decimal_length > max){
		fprintf(out, "%s", "LowPrec");
		return NULL;
	}
	changePrec(final);
    return final;
}

number* temp_multiply(number* a , int n)
{
	number* ans = (number*)malloc(sizeof(number));
    initialize(ans,0,0);
    node* curr = a->decimal_tail->prev;
    int carry = 0;
    while(curr->prev != NULL){
		int temp = (n * curr->digit) + carry;
      carry = temp/10;
      int no = temp%10;
      insert_front(no,ans->decimal_head);
      ans->decimal_length++;
	  curr = curr->prev;
    }
	if(carry != 0){
		insert_front(carry,ans->decimal_head);
		ans->decimal_length++;
	}
  if(ans->fractional_length == -1 || ans->fractional_length == 0){
  	ans->fractional_length = 1;
    insert_front(0,ans->fractional_head);
  }
  return ans;
}

void change_suitable_multiplication (number *a)
{

	if(a->fractional_length == 0){
		return;
	}
	int check = 0;
	node* curr = a->fractional_head->next;
	/*while(curr->next != NULL){
		if(curr->digit != 0)
		{
			check = 1;
			break;
		}
		curr = curr->next;
	}
	if(check == 1){*/
		curr = a->fractional_head->next;
		while(curr->next != NULL){
			insert_end(curr->digit, a->decimal_tail);
			a->decimal_length++;
			curr = curr->next;
		}
		a->fractional_head->next = a->fractional_tail;
		a->fractional_tail->prev = a->fractional_head;
		a->fractional_head->prev = NULL;
		a->fractional_tail->next = NULL;
		a->fractional_length = 1;
		insert_front(0,a->fractional_head);
	//}
}

void change_suitable_ans(number *a,int len)
{
	int i = 0;
	while(i<len && a->decimal_length != 0){
		insert_front(a->decimal_tail->prev->digit, a->fractional_head);
		a->fractional_length++;
		a->decimal_length--;
		a->decimal_tail->prev = a->decimal_tail->prev->prev;
		a->decimal_tail->prev->next = a->decimal_tail;
		i++;
	}
	if(i != len){
		while(i < len){
			insert_front(0,a->fractional_head);
			a->fractional_length++;
			i++;
		}
		insert_front(0,a->decimal_head);
		a->decimal_head++;
	}
}

number* multiplication (number c, number d)
{
	number* a = copy(c);
	number* b = copy(d);
	int final_fractional_length = a->fractional_length + b->fractional_length;
	change_suitable_multiplication(a);
	change_suitable_multiplication(b);
	node* curr = b->decimal_tail->prev;
	number* ans  = temp_multiply(a,curr->digit);
	curr = curr->prev;
	int i = 1;
	while(curr->prev != NULL){
		number* temp = temp_multiply(a,curr->digit);
		insert_zeroes_end(temp->decimal_tail, i);
        temp->decimal_length = temp->decimal_length + i;
		ans = addition(*ans, *temp);
		curr = curr->prev;
		i++;
	}
	change_suitable_ans(ans,final_fractional_length);
	remove_leading_zeroes(ans);
	remove_trailing_zeroes(ans);
	return  ans;
}
number* final_multiplication (number c, number d)
{
	number* a = copy(c);
	number* b = copy(d);
	int final_fractional_length = a->fractional_length + b->fractional_length;
	change_suitable_multiplication(a);
	change_suitable_multiplication(b);
	node* curr = b->decimal_tail->prev;
	number* ans  = temp_multiply(a,curr->digit);
	curr = curr->prev;
	int i = 1;
	while(curr->prev != NULL){
		number* temp = temp_multiply(a,curr->digit);
		insert_zeroes_end(temp->decimal_tail, i);
        temp->decimal_length = temp->decimal_length + i;
		ans = addition(*ans, *temp);
		curr = curr->prev;
		i++;
	}
	change_suitable_ans(ans,final_fractional_length);
	remove_leading_zeroes(ans);
	remove_trailing_zeroes(ans);
	if(ans->decimal_length > max){
        fprintf(out, "%s", "LowPrec");
		return NULL;
	}
	changePrec(ans);
	if(c.sign == 1 && d.sign == 0){
		ans->sign = 1;
		return ans;
	}
	else if(c.sign == 0 && d.sign == 1){
		ans->sign = 1;
		return ans;
	}
	else{
		ans->sign = 0;
	}
	return  ans;
}

int quotient_help(number* dividend, number* divisor)
{
    for (int i = 1; ; i++)
    {
        number* current1 = temp_multiply(divisor, i);
        int comp1 = compare(*current1, *dividend);
        if (comp1 == 0)
        {
            return i;
        }
        else if (comp1 == 1)
        {
            return i-1;
        }
    }
    return 0;
}

void change_suitable_division(number* a,int len)
{
	int i = 0;
	while(i < len && a->fractional_head->next->next != NULL){
		insert_end(a->fractional_head->next->digit, a->decimal_tail);
		a->decimal_length++;
		a->fractional_head->next = a->fractional_head->next->next;
		a->fractional_head->next->prev = a->fractional_head;
		a->fractional_length--;
		i++;
	}
	if(i != len){
		while(i<len){
			insert_end(0,a->decimal_tail);
			a->decimal_length++;
			i++;
		}
		insert_front(0,a->fractional_head);
		a->fractional_length++;
	}
}

number* remainder_help(number* a, number* b, int quo)
{
    number* current = temp_multiply(b, quo);
    number* sub = subtraction(*a, *current);
    return sub;
}

number* quotient(number c, number d)
{
    number* a = copy(c);
    number* b = copy(d);
    change_suitable_multiplication(a);
	change_suitable_multiplication(b);
    number* quotient = (struct number*) malloc(sizeof(struct number));
    initialize(quotient, 0, 0);
    quotient->fractional_length = 1;
    insert_end(0, quotient->fractional_tail);

    number* rem;

    number* temp = (struct number*) malloc(sizeof(struct number));
    initialize(temp, 0, 0);
    temp->fractional_length = 0;

    node* itr1 = a->decimal_head->next;
    for (size_t i = 0; i < a->decimal_length; i++)
    {
        insert_end(itr1->digit, temp->decimal_tail);
        temp->decimal_length++;
        int flag = compare(*temp, *b);
        if (flag < 0)
        {
            insert_end(0, quotient->decimal_tail);
            rem = copy(*temp);
        }
        else
        {
            int quo = quotient_help(temp, b);
            rem = remainder_help(temp, b, quo);
            temp = copy(*rem);
            insert_end(quo, quotient->decimal_tail);
        }
        quotient->decimal_length++;
        itr1 = itr1->next;
    }
	int final_len = - c.fractional_length + d.fractional_length;
	if(final_len < 0){
		change_suitable_ans(quotient, -final_len);
	}
	else if(final_len > 0){
		change_suitable_division(quotient, final_len);
	}
	remove_leading_zeroes(quotient);
	remove_trailing_zeroes(quotient);
    return quotient;
}

number* modulo(number a, number b)
{
    number* quo = quotient(a, b);
    number* temp = multiplication(*quo, b);
    number* m = subtraction(a, *temp);
    number* final = (struct number*) malloc(sizeof(struct number));
    initialize(final, 0, 0);
    final->fractional_length = 1;
    insert_end(0, final->fractional_tail);

    node* t = m->decimal_head->next;

    while(t->next != NULL)
    {
        insert_end(t->digit, final->decimal_tail);
		final->decimal_length++;
        t = t->next;
    }
	remove_leading_zeroes(final);
	remove_trailing_zeroes(final);
    return final;
}


number* division(number c, number d)
{
	if( compare(d,*convert_to_linked_list("0")) == 0){
		fprintf(out, "%s", "DivErr");
		exit(0);
	}
    number* a = copy(c);
    number* b = copy(d);
    change_suitable_multiplication(a);
	change_suitable_multiplication(b);
    number* quotient = (struct number*) malloc(sizeof(struct number));
    initialize(quotient, 0, 0);
    quotient->fractional_length = 0;

    number* rem;

    number* temp = (struct number*) malloc(sizeof(struct number));
    initialize(temp, 0, 0);
    temp->fractional_length = 0;

    node* itr1 = a->decimal_head->next;
    for (size_t i = 0; i < a->decimal_length; i++)
    {
        insert_end(itr1->digit, temp->decimal_tail);
        temp->decimal_length++;
        int flag = compare(*temp, *b);
        if (flag < 0)
        {
            insert_end(0, quotient->decimal_tail);
            rem = copy(*temp);
        }
        else
        {
            int quo = quotient_help(temp, b);
            rem = remainder_help(temp, b, quo);
            temp = copy(*rem);
            insert_end(quo, quotient->decimal_tail);
        }
        quotient->decimal_length++;
        itr1 = itr1->next;
    }
    int done = 0;
    for (size_t i = 0; i < 40; i++)
    {
		insert_end(0, rem->decimal_tail);
		rem->decimal_length++;
        int flag = compare(*rem, *b);
        if (flag < 0)
        {
            insert_end(0, quotient->fractional_tail);
            quotient->fractional_length++;
        }
        else
        {
            int quo = quotient_help(rem, b);
            rem = remainder_help(rem, b, quo);
            insert_end(quo, quotient->fractional_tail);
            quotient->fractional_length++;
            done = 0;
        }
    }
	int final_len = - c.fractional_length + d.fractional_length;
	if(final_len < 0){
		change_suitable_ans(quotient, -final_len);
	}
	else if(final_len > 0){
		change_suitable_division(quotient, final_len);
	}
	remove_leading_zeroes(quotient);
	remove_trailing_zeroes(quotient);
    return quotient;
}

number* final_division(number c, number d)
{
	if( compare(d,*convert_to_linked_list("0")) == 0){
		fprintf(out, "%s", "DivErr");
		exit(0);
	}
    number* a = copy(c);
    number* b = copy(d);
    change_suitable_multiplication(a);
	change_suitable_multiplication(b);
    number* quotient = (struct number*) malloc(sizeof(struct number));
    initialize(quotient, 0, 0);
    quotient->fractional_length = 0;

    number* rem;

    number* temp = (struct number*) malloc(sizeof(struct number));
    initialize(temp, 0, 0);
    temp->fractional_length = 0;

    node* itr1 = a->decimal_head->next;
    for (size_t i = 0; i < a->decimal_length; i++)
    {
        insert_end(itr1->digit, temp->decimal_tail);
        temp->decimal_length++;
        int flag = compare(*temp, *b);
        if (flag < 0)
        {
            insert_end(0, quotient->decimal_tail);
            rem = copy(*temp);
        }
        else
        {
            int quo = quotient_help(temp, b);
            rem = remainder_help(temp, b, quo);
            temp = copy(*rem);
            insert_end(quo, quotient->decimal_tail);
        }
        quotient->decimal_length++;
        itr1 = itr1->next;
    }
    int done = 0;
    for (size_t i = 0; i < 40; i++)
    {
		insert_end(0, rem->decimal_tail);
		rem->decimal_length++;
        int flag = compare(*rem, *b);
        if (flag < 0)
        {
            insert_end(0, quotient->fractional_tail);
            quotient->fractional_length++;
        }
        else
        {
            int quo = quotient_help(rem, b);
            rem = remainder_help(rem, b, quo);
            insert_end(quo, quotient->fractional_tail);
            quotient->fractional_length++;
            done = 0;
        }
    }
	int final_len = - c.fractional_length + d.fractional_length;
	if(final_len < 0){
		change_suitable_ans(quotient, -final_len);
	}
	else if(final_len > 0){
		change_suitable_division(quotient, final_len);
	}
	remove_leading_zeroes(quotient);
	remove_trailing_zeroes(quotient);
	if(quotient->decimal_length > max){
		fprintf(out, "%s", "LowPrec");
		return NULL;
	}
	changePrec(quotient);
	if(c.sign == 1 && d.sign == 0){
		quotient->sign = 1;
		return quotient;
	}
	else if(c.sign == 0 && d.sign == 1){
		quotient->sign = 1;
		return quotient;
	}
	else{
		quotient->sign = 0;
	}
    return quotient;
}

number* form_num(int temp)
{
	int len = temp/2 + 1;
	number *accuracy = (number *)malloc(sizeof(number));
	initialize(accuracy,0,0);
	accuracy->fractional_length = 0;
	int i;
	for(i=0;i<len-1;i++){
		insert_front(0,accuracy->decimal_head);
		accuracy->decimal_length++;
	}
	insert_front(1,accuracy->decimal_head);
	accuracy->decimal_length++;
	insert_front(0,accuracy->fractional_head);
	accuracy->fractional_length++;
	return accuracy;
}

number* form_one()
{
	number *accuracy = (number *)malloc(sizeof(number));
	initialize(accuracy,0,0);
	accuracy->fractional_length = 1;
	insert_front(1,accuracy->decimal_head);
	accuracy->decimal_length = 1;
	insert_front(0,accuracy->fractional_head);
	return accuracy;
}

number* form_zero()
{
	number *accuracy = (number *)malloc(sizeof(number));
	initialize(accuracy,0,0);
	accuracy->fractional_length = 1;
	insert_front(0,accuracy->decimal_head);
	accuracy->decimal_length = 1;
	insert_front(0,accuracy->fractional_head);
	return accuracy;
}

number* square_root(number c)
{
	if(c.sign == 1){
		fprintf(out, "%s", "SqrtErr");
		exit(0);
	}
	number* a = copy(c);
	number* accuracy = (number* )malloc(sizeof(number));
	initialize(accuracy,0,0);
	accuracy->fractional_length = 0;
	int i;
	for(i=0;i<3;i++){
		insert_end(0,accuracy->fractional_tail);
		accuracy->fractional_length++;
	}
	insert_end(1,accuracy->fractional_tail);
	accuracy->fractional_length++;
	insert_front(0,accuracy->decimal_head);
	accuracy->decimal_length++;
	remove_leading_zeroes(a);
	number *x;
	if(a->decimal_length == 1){
		x = a;
	}
	else{
		x = form_num(a->decimal_length);
	}
	number* one = form_one();
	number* two = addition(*one,*one);
	number* y = copy(*one);
	i = 0;

	while(compare(*subtraction(*y,*x),*accuracy) > 0){

		i++;

		remove_trailing_zeroes(x);
		remove_leading_zeroes(x);

		y = division(*a,*x);

		remove_trailing_zeroes(y);
		remove_leading_zeroes(y);



		x = addition(*x,*y);

		remove_trailing_zeroes(x);
		remove_leading_zeroes(x);
		x = division(*x,*two);


	}
	remove_leading_zeroes(x);
	remove_trailing_zeroes(x);
	if(x->decimal_length > max){
		fprintf(out, "%s", "LowPrec");
		return NULL;
	}
	changePrec(x);
	return x;
}

int check_fraction(number a){
	number* c = copy(a);
	remove_trailing_zeroes(c);
	if(c->fractional_length == 1 && c->fractional_head->next->digit == 0){
		return 0;
	}
	return 1;
}
number* power(number c,number d)
{

    number*x = copy(c);
	number* y = copy(d);
	int check = check_fraction(d);
	if( check == 1){
		number* ans = second_power(*x,*y);
		return ans;
	}
	number *zero = form_zero();
	number* one = form_one();
	number* two = addition(*one,*one);
    number* result = copy(*one);

    while ( compare(*y,*zero) > 0)
    {
        number* temp1 = modulo(*y,*two);
		if ( compare(*temp1,*zero) > 0){
			result = multiplication(*result,*x);
		}
		y = quotient(*y,*two); // y = y/2
		x = multiplication(*x,*x);  // Change x to x^2

    }
	remove_leading_zeroes(result);
	remove_trailing_zeroes(result);
    if(result->decimal_length > max){
		fprintf(out, "%s", "LowPrec");
		return NULL;
	}
	changePrec(result);
	return result;
}

number* second_power(number c, number d){
	number* a = copy(c);
	number* b = copy(d);
	number* temp = logarithm(*a);
	temp = multiplication(*temp, *b);
	number* constant = convert_to_linked_list("2.302585092994046"); //ln(10)
	temp = multiplication(*temp, *constant);
	number* ans = exponential(*temp);
	return ans;
}

number* logarithm(number a)
{
    number* g = copy(a);
    remove_leading_zeroes(g);
    int charec = g->decimal_length;
    char convert[charec];
    snprintf(convert, sizeof(convert), "%d", charec-1);
    number* characteristic = convert_to_linked_list(convert);

    number* ten = convert_to_linked_list("10");
    number* constant = convert_to_linked_list("2.302585092994046"); //ln(10)

    number* temp1 = power(*ten, *characteristic);
    number* temp2 = division(a, *temp1);

    number* one = convert_to_linked_list("1");
	number* two = convert_to_linked_list("2");
    number* c = copy(*temp2);
    number* d = copy(*temp2);
    c = subtraction(*c, *one);
    d = addition(*d, *one);
    temp1 = division(*c, *d);
    temp2 = multiplication(*temp1, *temp1);
    number* sum = copy(*temp1);
    number* temp3;
	char buffer[10];
    for (int i = 1; i < 100; i++)
    {
        float con = 1.0/(2*i + 1.0);
		snprintf(buffer, sizeof buffer, "%f", con);
		number* temp4 = convert_to_linked_list(buffer);
        temp1 = multiplication(*temp1,*temp2);
		temp3 = multiplication(*temp1, *temp4);
        sum = addition(*sum, *temp3);
    }
	sum = multiplication(*sum, *two);
    number* mantissa = division(*sum, *constant);
    number* final = addition(*mantissa, *characteristic);
    if(final->decimal_length > max){
		fprintf(out, "%s", "LowPrec");
		exit(0);
	}
	changePrec(final);
	return final;
}
 number* exponential(number a)
{
	number* in = copy(a);
	number* sum = convert_to_linked_list("1.0");
    for (int i = 9; i > 0; --i )
	{
		char str[3];
		sprintf(str, "%d", i);
		number* temp = division(*sum,*convert_to_linked_list(str));
		temp = multiplication(*temp,*in);
		sum = addition(*temp,*convert_to_linked_list("1.0"));
	}
	if(sum->decimal_length > max){
		fprintf(out, "%s", "LowPrec");
		exit(0);
	}
	changePrec(sum);
	return sum;
}

char* convert_to_string(number a){
	number *c = copy(a);
	int len = c->fractional_length + c->decimal_length + 2;
	char* ans = (char *)malloc(sizeof(char) * len);
	node* curr = c->decimal_head->next;
	int i = 0;
	if(a.sign == 1){
		ans[i] = '-';
	}
	else{
		ans[i] = '0';
	}
	i++;
	while(curr->next != NULL){
		ans[i] = curr->digit + '0';
		i++;
		curr = curr->next;
	}
	if(c->fractional_length > 0){
		ans[i] = '.';
		i++;
	}
	curr = c->fractional_head->next;
	while(curr->next != NULL){
		ans[i] = curr->digit + '0';
		i++;
		curr = curr->next;
	}
	return ans;

}
