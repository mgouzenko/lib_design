graph -T png\
	-W .0035 -C -m 1 vector.txt\
	-W .0035 -m 2 list.txt\
		--top-label "Vector VS Linked List"\
		--y-label "Milliseconds"\
		-w .7\
		--bitmap-size "1600x1200"\
		--frame-line-width .0035\
> plot.png 
