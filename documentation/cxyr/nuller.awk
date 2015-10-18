#! /usr/bin/awk -f

// {
	if (length ($1) == 1) {
		system("git mv '"$0"' '0"$0"'");
	}
}
