MINIPROJECT -ispell NAME- Rahul Keshervani MISID- 111603065

LINUX COMMAND "ispell" will be implemented in this c based project.

ITS FEATURES WILL BE AS FOLLOWS:

1)IT WILL CHECK MULTIPLE WORDS(SEPARATED BY SPACES) FOR SPELLING MISTAKES BY COMPARING THEM/IT TO ITS DICTIONARY Database.

2)IT WILL SUGGEST SOME OTHER WORDS IF THE ENTERED WORD IS NOT PRESENT IN THE DICTIONARY BUT HAS SOME SIMILAR WORDS.

3)IT WILL PRINT "OK" IF THE SPELLING IS TOTALLY RIGHT.

4)IT WILL PRINT "NOT FOUND" IF THERE ARE NO WORDS EVEN SIMILAR TO THE ENTERED WORD/S.

5)INPUT CAN ALSO BE A TEXT FILE.

command ispell without filename:
User needs to enter words or sentences and the ispell checks every word and the delimiters are defined in ispell.h


suggestions.c has the main logic for suggesting and comparing words to a dictionary created in dict.c

the dictionary created in dict.c is a hash table and the original dictionary is british-english


test file and snapshots of this version is named test_normal and snapshot1


ispell with file:
will read 1 line and show the misspelled word ine by one

Options provided are:
 R for replacing the current word
 space for ignoring the current word

number (if there are suggestions for the current word) : the suggested word's number is to entered

A for accepting the word for this session

I for accepting and putting it tot the dictionary file

U for  accepting and putting it tot the dictionary file in lowercase

Q for Quit

test files and snapshots are in the tar ball test-and-snapshots
