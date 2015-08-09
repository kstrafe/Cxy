#! /usr/bin/awk -f


/COMBINING/ {
	n = split($1, a, /\.\./);
	if (n > 1) {
		printf "if (code_point >= 0x" a[1] " and code_point <= 0x" a[2] ")";
	} else {
		printf "if (code_point == 0x" a[1] ")";
	}
	printf "\n\treturn false;\n";
}

