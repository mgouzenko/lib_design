if [ $# -lt 4 ]
	then
		echo "Usage: <Vector Infile> <List Infile> <Outfile Name> <Title>"
		exit
fi
graph -T png\
	-W .0035 -C -m 1 $1\
	-W .0035 -m 2 $2\
		--top-label "$4"\
		--title-font-size 0.05\
		-f 0.035\
		--y-label "Microseconds"\
		--x-label "Number of Elements"\
		-w .7\
		--bitmap-size "3200x2400"\
		--frame-line-width .0035\
> $3 
