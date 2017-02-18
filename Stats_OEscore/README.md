# Stats-for-Sequences
 C implementation of Statistical methods and their applications on Sequences.
 
 
 Stats - O/E score , a simple model for over and under represented words
 -----------------------------------------------------------------------
 Usage
 -----
 
 compiling the code :   gcc calcOEscore.c
 
 Note: on compiling there might be warnings on qsort() being used in the program. ignore it and execute. warning is due to the non-standand implementation of the qsort().
 
 execution :   ./a.out -min 3 -max 6 -desctot ./books/alice_in_wonderland_nows 
 
 Note :  the books are taken from the https://www.gutenberg.org/ . all the white spaces and preamble is removed.
 
 HELP
 ----
 
 you can see the various option for the program if you do not give any arguments.below is the options available.
 
 Usage: word_count -min X -max Y [-descseq|-ascseq|-desctot|-asctot|-desclength|-asclength] filename [other filenames]
 
-min minimum word length
-max maximum word length
-descseq causes the output to be sorted in descending order of sequence counts
-ascseq  causes the output to be sorted in ascending order of sequence counts
-desctot causes the output to be sorted in descending order of total counts
-asctot  causes the output to be sorted in ascending order of total counts
-desclength causes the output to be sorted in descending order of word length
-asclength  causes the output to be sorted in ascending order of word length

Example: word_count -min 3 -max 10 -asctot filename1 filename2 filname3

-----------------------------------------------------------------------------

 
 
