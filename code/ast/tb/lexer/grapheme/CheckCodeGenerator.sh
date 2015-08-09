cd $(dirname $0)
cat GraphemeBreakProperty.txt | ./codegenerator.awk > check.inc
