/*
#
# author : Prashant Kuntala
# date   : 17th Feb, 2017
#
# last modified : 17th Feb, 2017
#
*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define NHASH 263167 //Use a prime number!
#define MULT 31

#define MAX_WORD_LENGTH		15
#define MAX_NUMBER_OF_SEQUENCES 10
#define MAX_FILENAME_LENGTH	255
#define MAX_FILE_LENGTH 	500000

#define ALPHA_NUM 26
#define max(a,b) (((a) > (b)) ? (a) : (b))

struct node
{
// add length of the word
// add probability of the word or have a function that calculates it before printing.
	char *word;
	int total_count;
	int sequence_count;
	int last_matching_sequence_id;
	struct node * next;
} node;

typedef enum {
        ASCENDING_TOTAL,
        ASCENDING_SEQUENCE,
        DESCENDING_TOTAL,
        DESCENDING_SEQUENCE,
        ASCENDING_WORDLENGTH,
        DESCENDING_WORDLENGTH
} sort_order_t;


typedef struct node Node;
unsigned long total_number_words = 0;

Node *bin[NHASH];
Node** bin_array;

char input_sequences[MAX_NUMBER_OF_SEQUENCES][MAX_FILE_LENGTH];
unsigned long sequence_lengths[MAX_NUMBER_OF_SEQUENCES];

//record the p(a), p(b)...
double prob_letter[50];


unsigned int hash(char *p)
{
	unsigned int h = 0;
	for(; *p; p++)
		h = MULT * h + *p;
	return h % NHASH;
}

// calculates word frequencies.
void incword(char *s, int sequence_id)
{
	Node * p;
	int h = hash(s);
	for(p = bin[h]; p!= NULL; p = p->next)
	{
		if(strcmp(s, p->word) == 0)
		{
			p->total_count++;

        		if(sequence_id != p->last_matching_sequence_id)
	        	{
		                p->sequence_count++;
		                p->last_matching_sequence_id = sequence_id;
                        }

			return;
		}
	}

	p = (Node *)malloc(sizeof(node));
	if(!p)
		return;
	p->total_count = 1;
	p->sequence_count = 1;
	p->last_matching_sequence_id = sequence_id;
	p->word = (char *)malloc(strlen(s)+1);
	total_number_words++;
	strcpy(p->word, s);
	p->next = bin[h];
	bin[h] = p;

	//printf("mycount %s\t%d\n",p->word,p->total_count);
}




int compare( const void* ptr1, const void* ptr2, const void* ptr3)
{
        Node** np1, *(*np2);
        sort_order_t sort_order = (sort_order_t)ptr3;
        np1 = (Node**) ptr1;
        np2 = (Node**) ptr2;


        switch(sort_order)
        {
            case ASCENDING_TOTAL:
                    if( (*np1)->total_count < (*np2)->total_count )
                            return -1;
                    if( (*np1)->total_count == (*np2)->total_count )
                            sort_order = ASCENDING_WORDLENGTH;
                    if( (*np1)->total_count > (*np2)->total_count )
                            return 1;
                    break;

            case ASCENDING_SEQUENCE:
                    if( (*np1)->sequence_count < (*np2)->sequence_count )
                            return -1;
                    if( (*np1)->sequence_count == (*np2)->sequence_count )
                            return 0;
                    if( (*np1)->sequence_count > (*np2)->sequence_count )
                            return 1;
                    break;

            case DESCENDING_TOTAL:
                    if( (*np1)->total_count > (*np2)->total_count )
                            return -1;
                    if( (*np1)->total_count == (*np2)->total_count )
                            return 0;
                    if( (*np1)->total_count < (*np2)->total_count )
                            return 1;
                    break;

            case DESCENDING_SEQUENCE:
                    if( (*np1)->sequence_count > (*np2)->sequence_count )
                            return -1;
                    if( (*np1)->sequence_count == (*np2)->sequence_count )
                            return 0;
                    if( (*np1)->sequence_count < (*np2)->sequence_count )
                            return 1;
                    break;

            case ASCENDING_WORDLENGTH:
                    if( strlen( (*np1)->word ) < strlen( (*np2)->word ))
                            return -1;
                    if( strlen( (*np1)->word ) == strlen( (*np2)->word ))
                            return 0;
                    if( strlen( (*np1)->word ) > strlen( (*np2)->word ))
                            return 1;
                    break;

            case DESCENDING_WORDLENGTH:
                    if( strlen( (*np1)->word ) > strlen( (*np2)->word ))
                            return -1;
                    if( strlen( (*np1)->word ) == strlen( (*np2)->word ))
                            return 0;
                    if( strlen( (*np1)->word ) < strlen( (*np2)->word ))
                            return 1;
                    break;
            default:
                    return 0;
                    break;
        }
}

void sort_hash(sort_order_t order)
{
        Node* p;
        int i;
        unsigned long nwords = 0;

        bin_array = (Node**)malloc(total_number_words * (sizeof(Node*)));

        for(i = 0; i < NHASH; i++)
        {
                for(p = bin[i]; p!= NULL; p = p->next)
                {
                      bin_array[nwords++] = p;
                }

        }

        qsort_r(&bin_array[0], total_number_words, sizeof(Node*), compare, (void*)order);

}



void print_cmdline_error() {
	fprintf(stderr, "Error: improper command line arguments\n");
	fprintf(stderr, "Usage: word_count -min X -max Y [-descseq|-ascseq|-desctot|-asctot|-desclength|-asclength] filename [other filenames]\n");
	fprintf(stderr, "-min minimum word length\n");
	fprintf(stderr, "-max maximum word length\n");
	fprintf(stderr, "-descseq causes the output to be sorted in descending order of sequence counts\n");
	fprintf(stderr, "-ascseq  causes the output to be sorted in ascending order of sequence counts\n");
	fprintf(stderr, "-desctot causes the output to be sorted in descending order of total counts\n");
	fprintf(stderr, "-asctot  causes the output to be sorted in ascending order of total counts\n");
	fprintf(stderr, "-desclength causes the output to be sorted in descending order of word length\n");
	fprintf(stderr, "-asclength  causes the output to be sorted in ascending order of word length\n");
	fprintf(stderr, "Example: word_count -min 3 -max 10 -asctot filename1 filename2 filname3\n");
	exit(1);
}

// Function that read the filecontents into the buffer passed and returns the length of the file.
unsigned long open_copy(char* filename, char* cbuf)
{
	FILE *fp;
	fp = fopen(filename, "r");

	unsigned long sequence_length = 0;



        if( fp == NULL)
        {
                fprintf(stderr, "Error: unable to open file %s\n", filename);
                exit(1);
        }

        while( fgets(cbuf, MAX_FILE_LENGTH, fp) == NULL );


        return strlen(cbuf);

}
//########################################################## functions


// calculates the frequency of the word in the sequence
int calcfr(char* seq,char str, int len){

char dest[12];
memset(dest,'\0',sizeof(dest));
char src[2];
src[0] = str;
src[1] = '\0';

int count = 0;

for(int i=0;i<strlen(seq);i++){
strncpy(dest, seq+i,len);
if(strcmp(src,dest) == 0)
{
count = count+1;
}
}
//printf("word count: %d\t word: %s\n",count,src);
return count;
}

int calcfr2(char* seq,char* str, int len){

char dest[12];
memset(dest,'\0',sizeof(dest));

int count = 0;

for(int i=0;i<strlen(seq);i++){
strncpy(dest, seq+i,len);
if(strcmp(str,dest) == 0)
{
count = count+1;
}
}
//printf("word count: %d\t word: %s\n",count,src);
return count;
}

//calculates letter Probabilities

void calcLetterProb(int number_of_sequences,unsigned long *sequence_lengths)
{
char ch[10];
double val;
char letter;
int counts[40];	// to store frequency.
memset(&counts[0], 0, sizeof(counts));
int denom =-1;

//calculates the letter frequencies in all the books
 for (letter= 65 ; letter <= 90; ++letter)
	{
	counts[letter-65] = 0;
	for(int i =0;i<number_of_sequences;i++){

		counts[letter-65] = counts[letter-65]+ calcfr(&input_sequences[i][0],letter,1);
		denom += sequence_lengths[i];
	}
	//printf("letter: %c\t my total : %d\n",letter,counts[letter-65]);

    val = counts[letter-65];

	   prob_letter[letter-65] = val/denom;
 	 //  printf("Count of '%c' is  %f\t and probability is  %.15lf total chars : %d\n", letter,val, prob_letter[letter-65],denom);
	   denom = -1;


 }
	 	//printf("probability if A %lf\n",prob_letter[0]);
}




// function to calculate the word probabilities.
double calcWordProb(char* word)
{
double val =1.0;
char ch;
//printf("word : %s\n",word);
	for(int i = 0; word[i] != '\0'; i++) {
		ch = word[i];
		//printf("letter : %c \t prob %lf \n",ch,prob_letter[ch-65]);
		//printf("i: %d prob: %.10lf\n",i, prob_letter[ch-65]);
   	        val = val * prob_letter[ch-65];
		}
	// printf("word : %s \t prob %.15lf \n",word,val);

return val;

}



int main(int argc, char *argv[])
{
	int number_of_sequences = 0;
	int min_wordlength = 0;
	int max_wordlength = 0;
	sort_order_t sort_order = DESCENDING_TOTAL;
	int i,k;
	signed int j;
	double wordprob = 1;	// to store the value of word probability returned by calcWordProb
	double oescore ;	// to store the O/E score for each word.
	double expectedvalue =0;	// to store the denominatore (n-k+1)*P(w)
	if (argc == 1) {
		print_cmdline_error();
	}

	for (i = 1; i < argc; i++) {
		if (strcmp(argv[i], "-min") == 0) {  // input file
			++i;
			min_wordlength = atoi(argv[i]);

		} else if (strcmp(argv[i], "-asctot") == 0) {
			sort_order = ASCENDING_TOTAL;
		} else if (strcmp(argv[i], "-ascseq") == 0) {
                        sort_order = ASCENDING_SEQUENCE;
		} else if (strcmp(argv[i], "-desctot") == 0) {
			sort_order = DESCENDING_TOTAL;
		} else if (strcmp(argv[i], "-descseq") == 0) {
			sort_order = DESCENDING_SEQUENCE;
                } else if (strcmp(argv[i], "-asclength") == 0) {
			sort_order = ASCENDING_WORDLENGTH;
                } else if (strcmp(argv[i], "-desclength") == 0) {
			sort_order = DESCENDING_WORDLENGTH;
		} else if (strcmp(argv[i], "-max") == 0) {
			++i;
			max_wordlength = atoi(argv[i]);
		} else {
			sequence_lengths[number_of_sequences] = open_copy(argv[i], &input_sequences[number_of_sequences][0]);
			++number_of_sequences;
		}
	}


	if( min_wordlength == 0 || max_wordlength == 0 || min_wordlength > max_wordlength )
	{
		fprintf(stderr, "Error: incorrect or missing minimum or maximum word length\n");
		exit(1);
	}

	if( number_of_sequences == 0)
	{
		fprintf(stderr, "Error: no input file names specified\n");
		exit(1);
	}

	fprintf(stderr,"min wordlength = %d\n", min_wordlength);
	fprintf(stderr,"max wordlength = %d\n", max_wordlength);
	fprintf(stderr,"number of input files = %d\n", number_of_sequences);

	char buf[MAX_WORD_LENGTH+1];
	int wl;

	// Enumeration
	for( i=0; i < number_of_sequences; i++)
	{
                for( wl = min_wordlength; wl < max_wordlength+1; wl++)
                {
                	// read words of length wl from the sequence.
                        for( j=0; j < max( 0, (signed int)(sequence_lengths[i]-wl+1)); j++)
                        {
                                strncpy(buf, &input_sequences[i][j], wl);
                                buf[wl] = '\0';
                                incword(buf, i); // calculates each word frequency and also populates the hash.
                        }
                }
	}

	// Sort the output
        sort_hash(sort_order);

	calcLetterProb(number_of_sequences,sequence_lengths);

        printf("total number of words = %ld\n", total_number_words);
	printf("Word\tWordCount\t SeqCount\t Word Probbility\tExpected value\tO/E score \n");
        for(i = 0; i < total_number_words; i++)
        {
                if( bin_array[i] == NULL )
                {
                        exit(1);
                }
               //calculate the word probability.
               wordprob = calcWordProb(bin_array[i]->word);

              //printf("word : %s \t wordlen: %ld\t seqlen : %ld\t prob %.15lf \n",bin_array[i]->word,strlen(bin_array[i]->word),sequence_lengths[0],wordprob);
              //printf("no of sequences : %d\n",number_of_sequences );
              for(int j =0;j<number_of_sequences;j++){
               expectedvalue = expectedvalue + (wordprob * (sequence_lengths[j]-strlen(bin_array[i]->word)+1));
                //printf("word2 : %s \t denom %.15lf , sequence length : %ld\n",bin_array[i]->word,expectedvalue,sequence_lengths[j]);
                }
               oescore = bin_array[i]->total_count/expectedvalue;
               //printf("word3 : %s \t wordcount: %d\t prob %.15lf \n",bin_array[i]->word,bin_array[i]->total_count,oescore);
		// print out csv so that we can redirect output to a csv file.

              printf("%-*s \t%d \t%d\t %.15lf\t %.15lf\t %.15lf\n", max_wordlength, bin_array[i]->word, bin_array[i]->total_count, bin_array[i]->sequence_count,wordprob,expectedvalue,oescore);
             //printf("%-*s \t\t %d \t\t %d\t \n", max_wordlength, bin_array[i]->word, bin_array[i]->total_count, bin_array[i]->sequence_count);
        }

/* DEBUG
int c = 0;
//char* word = "A";
for(int i =0;i<=1;i++){
char* word = bin_array[i]->word;
c = c+ calcfr2(&input_sequences[i][0],word,1);

}
//printf("for word : %s my total : %d \n",word,c);
printf("my total : %d \n",c);
*/
// double m = calcWordProb("THE");
// printf("prob of THE : %f",m);

}
