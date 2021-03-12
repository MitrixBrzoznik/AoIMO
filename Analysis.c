/*! \mainpage Documentation - Analysis of incomparable multidimensional object
\section intro_analysis Introduction to an analysis
Multidimensional objects are incomparable. It is not easy to determine which of observations is "better". Therefore, it is necessary to create a ranking of multidimensional objects by linear ordering. The purpose of this method is to order objects from best to worst in terms of a predetermined criterion.
Ranking should meet the following conditions:
- each object has a two neighbors
- being a neighbor is a reflexive relationship
- only two object from the considered set have only one neighbor

There are multiple linear ordering methods like:
- standardized sum method (used in this program)
- Hellwig method
- TOPSIS method
- rank method

Standardized sum method algorithm:
- Converting all variables to stimulants
- Standardization of data (ensures the comparability of features)
- Adding estimates obtained from objects
- Standardization of the obtained values

\section intro_program Introduction to a program
\subsection user_interaction User interaction with the program
The program prompts the user to reference files that have the names of observations and variables and dataset. At the same time it checks whether:
- file exists
- file is not empty
- various files have been entered
- the data from the files are compatible with each other

Data from files are saved into arrays. Line breaks (needed for transparent printing to a file) are removed.
Then the program informs the user about the necessity to enter the value of the coefficient of variation. Depending on the value, the output file will contain information about the required removal of the variable (not necessary).
The program will ask the user to enter the name of the output file.
If the data has been prepared correctly, the program will display the message "Completed. Results stored in (file_name) file" and save the results to a file.

\subsection files Files
The observation file contains the names of observations that the user enters into the program.
The variable file contains the names of variables that the user enters into the program.
The data file contains numerical data that the user enters into the program (all already changed to stimulants).

It is important that the number of observations * number of the variables is equal to the number of data. Otherwise, the program will inform user about an error while downloading data.

\subsection no_data Determining the number of variables, observations and data
The number of particular parameters was obtained by incrementing the declared variable while reading the newline mark.

\subsection statistics Descriptive statistics
- Minimal value: determined by comparing the values ​​for individual columns, the declared variable is overwritten if a lower value is found
- Maximal value: determined by comparing the values ​​for individual columns, the declared variable is overwritten when a higher value is found
- Mean value: calculated by summing all values ​​for the given column and dividing by the total number of observations
- Median value: calculated on the basis of a sorted data array, in the case of an even number of observations the mean of the middle 2 values, in the case of an odd number of observations the middle value
- Standard deviation value: rooted quotient of the square of the sum of differences of individual values ​​of a given variable with the mean value of the variable and the number of variables
- Variance: the square value of the standard deviation
- Coefficient of variation: the absolute value of the quotient of the variable's standard deviation and the mean values ​​of the variable presented as percentages

\subsection scaling Scaling variables
Scaling is done by taking the value and subtracting the mean value of the variable and dividing the obtained value by the variable's standard deviation.

\subsection rankings Create rankings
- Mean value: calculated by summing all values ​​for a given row and dividing by the total number of variables
- Minimum value: determined by comparing the value of an array of the mean value of each observation
- Maximum value: determined by comparing the value of an array of the mean value of each observation
- Index value: calculated by the quotient between the difference of the mean and the minimum value and the difference between the maximum and minimum value, the range of values ​​[0,1]

\subsection err Error handlers
- File existence - checks if the pointer is NULL, if so, it reports an EXISTENCE ERROR error
- File incorrect - checks if the user tried to enter the same file twice, if so, it informs about a DUPLICATE FILE ERROR error
- Data incorrect - makes the number of variables * number of observations different, if so, it informs about the DATA ERROR error
- Empty file - checks if a given file is empty, if so, it informs about EMPTY FILE ERROR
- Correctness of data: checks if data is not a number, if so, it informs about the DATA FORMAT ERROR error
- Correctness of the coefficient of variation: checks if the coefficient is not a number, if so, it informs about the DATA FORMAT ERROR error and if the coefficient takes a negative value, if so, it informs about the RANGE ERROR error
*/

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

/**
A macro, which is the association of an identifier or parameterized identifier with a token string.
*/
#define MEMORY 102

/**
An integer number of observations counted by function counter().
*/
int observation_number;

/**
An integer number of variables counted by function counter().
*/
int variables_number;

/**
An integer number of data counted by function counter().
*/
int data_number;

/**
A function that takes a parameter and returns the number of positions in a chosen file or error generated by errhl_no_data().
\param *file - a pointer to a FILE structure
*/
int counter(FILE *file);

/**
An error handler that takes a parameter and returns an error when file does not exist.
\param *file - a pointer to a FILE structure
*/
void errhl_file_exist(FILE*file);

/**
An error handler that takes 2 parameters and returns an error when tried to open the same file twice.
\param *file_name - a pointer to a first file name provided
\param *file_name2 - a pointer to a second file name provided
*/
void errhl_incorrect_file(char *file_name, char *file_name2);

/**
An error handler that returns an error when number of data does not match number of observations and variables.
*/
void errhl_incorrect_data();

/**
An error handler that returns an error when file is empty.
*/
void errhl_no_data();

/**
An error handler that returns a coeff value when provided coefficient by user is a positive float number, otherwise an error.
*/
float errhl_coeff();

/**
A function that takes 2 parameters and returns data value, otherwise an error.
\param data_file - a pointer to a FILE structure
\param i - position in file
*/
float errhl_data(FILE*data_file, int i);

/**
A function that takes 2 parameters and returns the minimum value of a given variable.
\param data[observation_number][variables_number] - a float dataset
\param i - an integer variable ID
\see observation_number
\see variables_number
*/
float f_minimum(float data[observation_number][variables_number], int i);

/**
A function that takes 2 parameters and returns the maximum value of a given variable.
\param data[observation_number][variables_number] - a float dataset
\param i - an integer variable ID
\see observation_number
\see variables_number
*/
float f_maximum(float data[observation_number][variables_number], int i);

/**
A function that takes 2 parameters and returns the mean value of a given variable.
\param data[observation_number][variables_number] - a float dataset
\param i - an integer variable ID
\see observation_number
\see variables_number
*/
float f_mean(float data[observation_number][variables_number], int i);

/**
A function that takes 2 parameters and returns the median value of a given variable.
\param data_copy[observation_number][variables_number] - a float sorted dataset
\param i - an integer variable ID
\see observation_number
\see variables_number
*/
float f_median(float data[observation_number][variables_number], int i);

/**
A function that takes 3 parameters and returns the standard deviation value of a given variable.
\param data[observation_number][variables_number] - a float dataset
\param mean_result[variabes_number] - a float array of mean values of variables
\param i - an integer variable ID
\see observation_number
\see variables_number
*/
float f_standard_deviation(float data[observation_number][variables_number], float mean_result[variables_number], int i);

/**
A function that takes 3 parameters and returns the coefficient of variation value of a given variable.
\param mean_result[variabes_number] - a float array of mean values of variables
\param sd_result[variales_number] - a float array of standard deviation values of variables
\param i - an integer variable ID
\see variables_number
*/
float f_coeff_of_variation(float mean_result[variables_number], float sd_result[variables_number], int i);

/**
A function that takes 5 parameters and returns the scaled dataset.
\param data[observation_number][variables_number] - a float dataset
\param mean_result[variabes_number] - a float array of mean values of variables
\param sd_result[variales_number] - a float array of standard deviation values of variables
\param i - an integer column ID in dataset
\param j - an integer row ID in dataset
\see observation_number
\see variables_number
*/
float f_scale(float data[observation_number][variables_number], float mean_result[variables_number], float sd_result[variables_number], int i, int j);

/**
A function that takes 2 parameters and returns the mean value of given observation.
\param data[observation_number][variables_number] - a float dataset
\param i - an integer observation ID
\see observation_number
\see variables_number
*/
float ranking_f_mean(float data[observation_number][variables_number], int i);

/**
A function that takes a parameter and returns the minimum value of mean values in observations array.
\param ranking_mean_result[observation_number] - a float array of mean values
\see observation_number
*/
float ranking_f_minimum(float ranking_mean_result[observation_number]);

/**
A function that takes a parameter and returns the maximum value of mean values in observations array.
\param ranking_mean_result[observation_number] - a float array of mean values
\see observation_number
*/
float ranking_f_maximum(float ranking_mean_result[observation_number]);

/**
A function that takes 4 parameters and returns the index value of an observation.
\param ranking_mean_result[observation_number] - a float array of mean values
\param ranking_minimum_result - a result of ranking_f_minimum()
\param ranking_maximum_result - a result of ranking_f_maximum()
\param i - an integer observation ID
\see observation_number
*/
float ranking_f_index(float ranking_mean_result[observation_number], float ranking_minimum_result, float ranking_maximum_result, int i);

/**
A function that gather data from user entered .txt file, creates a two-dimensional array and its copy sorted by variables and returns descriptive statistics and observations rank to the .txt file with a used-defined name.
*/
int main(){
    extern int observation_number,variables_number,data_number;
    int i, j, k;
    float temp;
    char observations_database[MEMORY], variables_database[MEMORY], data_database[MEMORY], results[MEMORY], temp2[MEMORY];

    printf("Provide file name (observation file) (e.g. Obs.txt): ");
    scanf("%s", observations_database);
    FILE *observation_file=fopen(observations_database, "r");
    errhl_file_exist(observation_file);

    observation_number = counter(observation_file);
    fseek(observation_file, 0, SEEK_SET);
    char observation[observation_number][MEMORY];
    for(i=0; i < observation_number; i++){
        fgets(observation[i], MEMORY, observation_file);
        if(observation[i][strlen(observation[i])-1]=='\n'){
            observation[i][strlen(observation[i])-1]='\0';
        }
    }
    fclose(observation_file);

    printf("Provide file name (variable file) (e.g. Var.txt): ");
    scanf("%s", variables_database);
    FILE *variables_file=fopen(variables_database, "r");
    errhl_file_exist(variables_file);
    errhl_incorrect_file(variables_database, observations_database);

    variables_number = counter(variables_file);
    fseek(variables_file, 0, SEEK_SET);
    char variable[variables_number][MEMORY];
    for(i=0; i < variables_number; i++){
        fgets(variable[i], MEMORY, variables_file);
        if(variable[i][strlen(variable[i])-1]=='\n'){
            variable[i][strlen(variable[i])-1]='\0';
        }
    }
    fclose(variables_file);

    printf("Provide file name (data file) (e.g. Data.txt): ");
    scanf("%s", data_database);
    FILE *data_file=fopen(data_database, "r");
    errhl_file_exist(data_file);
    errhl_incorrect_file(data_database, observations_database);
    errhl_incorrect_file(data_database, variables_database);

    data_number = counter(data_file);
    errhl_incorrect_data();

    fseek(data_file,0,SEEK_SET);

    float data_download[data_number];
    for(i=0; i <data_number; i++){
        data_download[i]=errhl_data(data_file, i);
    }

    fclose(data_file);

    float data[observation_number][variables_number];
    for(i=0;i<variables_number;i++){
        for(j=0;j<observation_number;j++){
            data[j][i]=data_download[k];
            k++;
        }
    }

    float data_copy[observation_number][variables_number];
    for(i=0; i<variables_number;i++){
        for(j=0;j<observation_number; j++){
            data_copy[j][i]=data[j][i];
        }
    }

    float min_result[variables_number], max_result[variables_number], mean_result[variables_number], median_result[variables_number], sd_result[variables_number],var_result[variables_number], coeff_of_var_result[variables_number], ranking_mean_result[observation_number], ranking_index_result[observation_number], min_coeff;

    printf("Provide minimal coeff value (e.g. 10%% = 10): ");
    min_coeff =errhl_coeff();

    printf("Provide file name (results file) (e.g. Results.txt): ");
    scanf("%s", results);

    FILE *results_file=fopen(results, "w");

    for(i=0;i<variables_number;i++)
    {
        fprintf(results_file, "Variable: %s\n", variable[i]);
        min_result[i]=f_minimum(data,i);
        max_result[i]=f_maximum(data,i);
        mean_result[i]=f_mean(data,i);
        median_result[i]=f_median(data_copy,i);
        sd_result[i]=f_standard_deviation(data, mean_result, i);
        var_result[i]=pow(f_standard_deviation(data, mean_result, i),2);
        coeff_of_var_result[i]=f_coeff_of_variation(mean_result, sd_result, i);

        if (coeff_of_var_result[i]<min_coeff){
            fprintf(results_file, "NOTE: Required to remove variable %s due to low level of coefficient of variation\n", variable[i]);
        }
        fprintf(results_file, "Minimum: %f\nMaximum: %f\nMean: %f\nMedian: %f\nStandard deviation: %f\nVariance: %f\nCoefficient of variation (%%): %f\n\n" ,min_result[i],max_result[i],mean_result[i],median_result[i],sd_result[i],var_result[i],coeff_of_var_result[i]);
        for(j=0;j<observation_number;j++){
            data[j][i]=f_scale(data, mean_result, sd_result, i ,j);
        }
    }

    for(i=0;i<observation_number;i++){
        ranking_mean_result[i]=ranking_f_mean(data, i);
    }

    for(i=0; i<observation_number;i++){
        ranking_index_result[i]=ranking_f_index(ranking_mean_result, ranking_f_minimum(ranking_mean_result), ranking_f_maximum(ranking_mean_result), i);
    }

    for(i=0; i<observation_number-1; i++){
        for(j=i+1; j<observation_number; j++){
            if(ranking_index_result[j]>ranking_index_result[i]){
                temp = ranking_index_result[j];
                ranking_index_result[j] = ranking_index_result[i];
                ranking_index_result[i] = temp;

                strcpy(temp2, observation[j]);
                strcpy(observation[j], observation[i]);
                strcpy(observation[i], temp2);
            }
        }
    }

    fprintf(results_file, "\nRANKING\n");
    for(i=0; i<observation_number;){
        fprintf(results_file, "%d. %f - %s\n", ++i, ranking_index_result[i], observation[i]);
    }
    fclose(results_file);



    printf("\nCompleted. Results stored in %s file\n", results);
    return 0;
}

float f_minimum(float data[observation_number][variables_number], int i){
    float minimum =data[0][i];
    int j;

    for(j=1; j<observation_number;j++){
        if(data[j][i]<minimum){
            minimum=data[j][i];
        }
    }
    return minimum;
}

float f_maximum(float data[observation_number][variables_number], int i){
    float maximum = data[0][i];
    int j;

    for(j=1; j<observation_number;j++){
        if(data[j][i]>maximum){
            maximum=data[j][i];
        }
    }
    return maximum;
}

float f_mean(float data[observation_number][variables_number], int i){
    float mean, sum=0;
    int j;

    for(j=0; j<observation_number;j++){
        sum+=data[j][i];
    }
    mean=sum/observation_number;
    return mean;
}

float f_median(float data_copy[observation_number][variables_number], int i){
    float median, temp;
    int j, k;

    median = 0;
    for(j=0; j<observation_number-1; j++){
        for(k=j+1; k<observation_number; k++){
            if(data_copy[j][i]<data_copy[k][i]){
                temp = data_copy[j][i];
                data_copy[j][i] = data_copy[k][i];
                data_copy[k][i] = temp;
            }
        }
    }
    if(observation_number%2==0){
        median = (data_copy[observation_number/2][i]+data_copy[observation_number/2-1][i])/2;
        return median;
    }
    else{
        median = data_copy[observation_number/2][i];
        return median;
    }
}

float f_standard_deviation(float data[observation_number][variables_number], float mean_result[variables_number], int i){
    float sd=0;
    int j;

    for (j=0; j<observation_number; j++){
        sd += pow(data[j][i]-mean_result[i],2);
    }
    sd=sqrt(sd/observation_number);
    return sd;
}

float f_coeff_of_variation(float mean_result[variables_number], float sd_result[variables_number], int i){
    float coeff_of_var = fabs(sd_result[i]/mean_result[i]*100);
    return coeff_of_var;
}

float f_scale(float data[observation_number][variables_number], float mean_result[variables_number],  float sd_result[variables_number], int i, int j){
    float scaled_value = (data[j][i]-mean_result[i])/sd_result[i];
    return scaled_value;
}

float ranking_f_mean(float data[observation_number][variables_number], int i){
    float mean, sum=0;
    int j;

    for(j=0; j<variables_number;j++){
        sum+=data[i][j];
    }
    mean=sum/variables_number;
    return mean;
}

float ranking_f_minimum(float ranking_mean_result[observation_number]){
    float minimum;
    int i;

    for(i=0;i<observation_number;i++){
        if(ranking_mean_result[i]<minimum){
            minimum=ranking_mean_result[i];
        }
    }
    return minimum;
}

float ranking_f_maximum(float ranking_mean_result[observation_number]){
    float maximum;
    int i;

    for(i=0; i<observation_number;i++){
        if(ranking_mean_result[i]>maximum){
            maximum=ranking_mean_result[i];
        }
    }
    return maximum;
}

float ranking_f_index(float ranking_mean_result[observation_number], float ranking_minimum_result, float ranking_maximum_result, int i){
    float index;

    index=(ranking_mean_result[i]-ranking_minimum_result)/(ranking_maximum_result - ranking_minimum_result);

    return index;
}

int counter(FILE *file){
    char character;
    int no=1;

    character=getc(file);
    if(character==EOF){
        errhl_no_data();
    }
    while(character!=EOF){
        character=getc(file);
        if(character=='\n'){
            no++;
        }
    }
    return no;
}

void errhl_file_exist(FILE*file){
    if (file==NULL){
        perror("\nEXISTENCE ERROR");
        exit(1);
    }
}

void errhl_incorrect_file(char *file_name, char *file_name2){
    if (strcmp(file_name,file_name2)==0){
        printf("\nDUPLICATE FILE ERROR: Tried to open the same file \n");
        exit(1);
    }
}

void errhl_incorrect_data(){
    if(observation_number*variables_number!=data_number){
        printf("\nDATA ERROR: Incorrect data - Number of observations*variables does not match number of data\nNumber of observations: %d\nNumber of variables: %d\nNumber of data: %d\n", observation_number, variables_number, data_number);
        exit(1);
    }
}

void errhl_no_data(){
    printf("\nEMPTY FILE ERROR: This file is empty\n");
    exit(1);
}

float errhl_coeff(){
    float min_coeff;
    if(scanf("%f", &min_coeff)!=1){
        printf("\nDATA FORMAT ERROR: Value is not a number \n");
        exit(1);
    }
    if (min_coeff<0){
        printf("\nRANGE ERROR: Coefficient cannot be lower than 0\n");
        exit(1);
    }
    return min_coeff;
}

float errhl_data(FILE*data_file, int i){
    float value;
    if(fscanf(data_file, "%f", &value)!=1){
        printf("\nDATA FORMAT ERROR: Value in position %d is not a number or there is an empty row\n",i+1);
        exit(1);
    }
    return value;
}
