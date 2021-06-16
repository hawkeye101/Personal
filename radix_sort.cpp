/* INPUT IS IN FORM OF STRING!!! */

#include <iostream>
#include <math.h>
#include <sstream>
using namespace std;

#define max_supremum 50

void print_mat(int matrix[][max_supremum], int row)
{
    int i, j;
    for (i = 0; i < row; ++i)
    {
        for (j = 0; j < max_supremum; ++j)
            cout<<matrix[i][j]<<" ";
        cout<<endl;
    }
}

int max_val(int input[], int len)
{
    int i;
    int max = input[0];
    int index = 0;
    for(i = 1; i < len; i++)
    {
        if(input[i] > max)
        {
            max = input[i];
            index = i;
        }
    }
    return max;
}

void convert_to_digit(double *input, int len, int num_digits_before[], int num_digits_after[], int digits_before[][max_supremum], int digits_after[][max_supremum], int digits[][max_supremum], int return_arr[])
{
    int i,j,k;
    
    int num_digits[len]; //, num_digits_before[len], num_digits_after[len];
    for(i = 0; i < len; i++)
    {
        num_digits[i] = num_digits_before[i] + num_digits_after[i];
    }
    int max_before = max_val(num_digits_before, len);
    int max_after = max_val(num_digits_after, len);
    int max = max_before + max_after;

//    int digits_before[len][max_before], digits_after[len][max_after];
    
    int count_before, count_after;
    for(i = 0; i < len ; i++)
    {        
        count_before = 0;
        count_after = 0;
        for(j = 0; j < max; j++)
        {
            if(j < max_before - num_digits_before[i])
                digits[i][j] = 0;
            else if(j < max_before)
                digits[i][j] = digits_before[i][count_before++];
            else if(j < max_before + num_digits_after[i])
                digits[i][j] = digits_after[i][count_after++];
            else
                digits[i][j] = 0;
        }
    }
    return_arr[0] = max;
    return_arr[1] = max_before;
    return_arr[2] = max_after;
}

void counting_sort(int len, int k, int digits[][max_supremum])
{
    int i, j, count[10]={};

    int arr_to_sort[len];
    for(i = 0; i < len; i++)
        arr_to_sort[i] = digits[i][k];

    for(i = 0; i < len; i++)
        count[arr_to_sort[i]]++;
        
    int count_pointer[10];
    count_pointer[0] = 0;
    for(i = 1; i < 10; i++)
        count_pointer[i] = count_pointer[i-1] + count[i-1];

    int digits_copy[len][max_supremum];
    int row;
    for(i = 0; i < len; i++)
    {
        row = count_pointer[digits[i][k]]++;
        
        for(j = 0; j < max_supremum; j++)
            digits_copy[row][j] = digits[i][j];
    }
    
    for(i = 0; i < len; i++)
        for(j = 0; j < max_supremum; j++)
            digits[i][j] = digits_copy[i][j];
}

void reverse_engineer(int digits[][max_supremum], int len, int max_data[], double output[])
{
    int i,j;
    for(i = 0; i < len; i++)
    {
        output[i] = 0;
        for(j = 0; j < max_data[0]; j++)
        {
            output[i] += (double)digits[i][j] * pow(10, max_data[1] - j - 1);
        }
    }
}

void digit_splitter(string arr[], int len, int digits_before[][max_supremum], int digits_after[][max_supremum], int num_digits_before[], int num_digits_after[])
{
    int i,j;
    for(i = 0; i < len; i++)
    {
        string s = arr[i];
	string delimiter = ".";

        size_t pos = 0;
        string token;
        string before, after;
        while ((pos = s.find(delimiter)) != string::npos)
        {
            token = s.substr(0, pos);
            before = token;

            s.erase(0, pos + delimiter.length());
        }
        after = s;

        num_digits_before[i] = before.size();
        num_digits_after[i] = after.size();

        for(j = 0; j < num_digits_before[i]; j++)
        {
            digits_before[i][j] = (int)(before[j] - '0');
        }
        for(j = 0; j < num_digits_after[i]; j++)
        {
            digits_after[i][j] = (int)(after[j] - '0');
        }
    }
}

void convert_string_to_double(string arr[], int len, double input[])
{
    int i;
    for(i = 0; i < len; i++)
    {
        istringstream(arr[i]) >> input[i];
    }
}

// input should be 1D array of double
void radix_sort(string arr[], int len, double output[])
{
    int i,j,k;

    int digits_before[len][max_supremum], digits_after[len][max_supremum];
    int num_digits_before[len], num_digits_after[len];
    digit_splitter(arr, len, digits_before, digits_after, num_digits_before, num_digits_after);

    int digits[len][max_supremum];

    double input[len];
    convert_string_to_double(arr, len, input);
    
    int max_data[3];
    convert_to_digit(input, len, num_digits_before, num_digits_after, digits_before, digits_after, digits, max_data);

    // Sort by each significant figure
    for(k = max_data[0] - 1; k >= 0; k--)
    {
        counting_sort(len, k, digits);
    }
    reverse_engineer(digits, len, max_data, output);
}

int main()
{
	// ENSURE THERE IS A DECIMAL CHARACTER!!!
	string arr[] = {"3.","4.02","4.01","1.","8.","6.","6.002","6.0002"};
	int len = sizeof(arr)/sizeof(arr[0]);

	double sorted_arr[len];

	radix_sort(arr, len, sorted_arr);
	//for(int i = 0; i < len; i++)
	//	cout << sorted_arr[i] << endl;

	return 0;
}

