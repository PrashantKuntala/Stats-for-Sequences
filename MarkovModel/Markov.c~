/*
#
# author : Prashant Kuntala
# date   : 17th Feb, 2017
#
# last modified : 6th April, 2017
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

int MARKOV_ORDER = 1;

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
  fprintf(stderr, "-order  pick the markov order 1, 2 or 3\n");
  fprintf(stderr, "Example: word_count -min 3 -max 10 -asctot -order 2 filename1 filename2 filname3\n");
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


// calculates the frequency of the letter in the sequence
int calcfr(char* seq,char str, int len){

char dest[12];
memset(dest,'\0',sizeof(dest));
char src[2];
src[0] = str;
src[1] = '\0';

int count = 0;
int  i =0;
for(i=0;i<strlen(seq);i++){
strncpy(dest, seq+i,len);
if(strcmp(src,dest) == 0)
{
count = count+1;
}
}
//printf("word count: %d\t word: %s\n",count,src);
return count;
}

// calculates the frequency of the word in the sequence(book in this case)
int calcWordFrequency(char* seq,char* str, int len){

char dest[12];
memset(dest,'\0',sizeof(dest));

int count = 0;
int i =0;
for( i=0;i<strlen(seq);i++){
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
int i = 0 ;
//calculates the letter frequencies in all the books
 for (letter= 65 ; letter <= 90; ++letter)
	{
	counts[letter-65] = 0;
	for(i =0;i<number_of_sequences;i++){

		counts[letter-65] = counts[letter-65]+ calcfr(&input_sequences[i][0],letter,1);
		denom += sequence_lengths[i];
	}
    val = counts[letter-65];
	   prob_letter[letter-65] = val/denom;
	   denom = -1;
 }
}




// function to calculate the word probabilities.
double calcWordProb(char* word)
{
double val =1.0;
char ch;
int i =0;
//printf("word : %s\n",word);
	for(i = 0; word[i] != '\0'; i++) {
		ch = word[i];
   	        val = val * prob_letter[ch-65];
		}
return val;

}

double phicount(char* word,int model,int number_of_sequences){

int i,j =0;
int cn,cd = 0; // numerator count and denominator count
char ch[model] ; // to retrieve the probability of the first letter.
char num[model+2];      // stores the substrings for the numerator.
char den[model];      // stores the substrings for the denominator.
Node * p;
int n=0;
if(model == 1){
  ch[0] = word[0];
  ch[1] = '\0';
}
else{
  strncpy(ch,word,model); // copying the first "m"  characters, m represents the MARCOV_ORDER

}
double wprob = calcWordProb(ch); // to store the final probability value.
memset(num,'\0',sizeof(num));
memset(den,'\0',sizeof(den));


// generate all substrings based on the model passed.
for(i=0;i<strlen(word);i=i+1){
 strncpy(num, word+i,model+1);
 strncpy(den, word+i,model);

  if(strlen(num)>model){

  cn = 0;
  cd = 0;

  n = hash(num);
  for(p = bin[n]; p!= NULL; p = p->next)
  {
    if(strcmp(num, p->word) == 0)
    {
      cn = p->total_count;
    }
  }

  n = hash(den);
  for(p = bin[n]; p!= NULL; p = p->next)
  {
    if(strcmp(den, p->word) == 0)
    {
      cd = p->total_count;
    }
  }
  wprob =  wprob * (double)((double)cn/(double)cd);
  // printf("numerator : %s\t cn : %d\t denominator: %s\t  cd: %d\t final wprob : %.15lf\n\n\n",num,cn,den,cd,wprob);
 }
}
// printf("final wprob : %.15lf\n",wprob);

return wprob;
}



int main(int argc, char *argv[])
{
	int number_of_sequences = 0;
	int min_wordlength = 0;
	int max_wordlength = 0;
	sort_order_t sort_order = DESCENDING_TOTAL;
	int i,k;
	int j;
	double wordprob = 1;	// to store the value of word probability returned by calcWordProb
	double oescore ;	// to store the O/E score for each word.
	double expectedvalue =0;	// to store the denominatore (n-k+1)*P(w)
  double totalSeqLength = 0; // to store the total sequence length ( since we have more than one sequence)
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
		}
   else if (strcmp(argv[i], "-order") == 0) {
      ++i;
      MARKOV_ORDER = atoi(argv[i]);
  }
     else {
			sequence_lengths[number_of_sequences] = open_copy(argv[i], &input_sequences[number_of_sequences][0]);
			++number_of_sequences;
		}
	}


  if (MARKOV_ORDER != 1 && MARKOV_ORDER != 2 && MARKOV_ORDER != 3 && MARKOV_ORDER != 4) {
      printf("%d\n", MARKOV_ORDER);
      fprintf(stderr, "Error: -order can take only  1, 2, 3 or 4\n");
      exit(1);
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
  fprintf(stderr,"Markov order chosen = %d\n", MARKOV_ORDER);


	char buf[MAX_WORD_LENGTH+1];
	int wl;

	// Enumeration
	for( i=0; i < number_of_sequences; i++)
	{
                for( wl = 1; wl < max_wordlength+1; wl++)
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
  for(j =0;j<number_of_sequences;j++){
   totalSeqLength = totalSeqLength + sequence_lengths[j] ;
    }
        printf("total number of words =\t %ld\n", total_number_words);
	       printf("Word\tWordCount\t SeqCount\t Word Probbility\tExpected value\tO/E score \n");
        for(i = 0; i < total_number_words; i++)
        {
                if( bin_array[i] == NULL )
                {
                        exit(1);
                }
            if(strlen(bin_array[i]->word) >= min_wordlength){

              wordprob = phicount(bin_array[i]->word,MARKOV_ORDER,number_of_sequences);

              expectedvalue = (wordprob * (totalSeqLength-strlen(bin_array[i]->word)+1));
              oescore = (double)bin_array[i]->total_count/(double)expectedvalue;

              printf("%-*s \t%d \t%d\t %.15lf\t %.15lf\t %.15lf\n", max_wordlength, bin_array[i]->word, bin_array[i]->total_count, bin_array[i]->sequence_count,wordprob,expectedvalue,oescore);

           }
        }


}
