# last2pass
I didnt want to install ruby to get [this](https://git.zx2c4.com/password-store/tree/contrib/importers/lastpass2pass.rb) working, so i made an importer myself in c
Might be very buggy. Semi-customizable
##Usage:

./last2pass file.csv

It can also read from stdin, if no file is specified

cat lastpassexport.csv | ./last2pass
