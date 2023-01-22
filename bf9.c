#include <u.h>
#include <libc.h>
#include <bio.h>

#define MEMSIZ 65535
#define PROGSIZ 4096

char memory[MEMSIZ];
char program[4096];

void
run(char* prog)
{
	int bf;
	char c;
	char* dp;
	Biobuf bstdin;

	if(Binit(&bstdin, 0, OREAD) == Beof)
		sysfatal("can’t connect stdin to bio: %r");

	dp = &memory[0];
	while (c = *prog++)
		switch (c){
		case '>':
			dp++;
			break;
		case '<':
			dp--;
			break;
		case '+':
			(*dp)++;
			break;
		case '-':
			(*dp)--;
			break;
		case '.':
			print("%c", *dp);
			break;
		case ',':
			// TODO
			*dp=Bgetc(&bstdin);
			break;
		case '[':
			if(!*dp){
				for (bf=1; bf; prog++){
					if(*prog == '[')
						bf++;
					else if(*prog == ']')
						bf--;
				}
			}
			break;
		case ']':
			if(*dp){
				prog -= 2;
				for(bf=1; bf; prog--){
					if(*prog == ']')
						bf++;
					else if (*prog == '[')
						bf--;
				}
				prog++;
			}
			break;
	}
	Bterm(&bstdin);
}

void
load(char* path)
{
	Biobuf* buf;
	int i;
	char c;

	if((buf = Bopen(path, OREAD)) == nil)
		sysfatal("Bopen: %r");
	i=0;
	while((c = Bgetc(buf)) != Beof)
		program[i++]=c;
	Bterm(buf);
}

void
main(int argc, char* argv[])
{
	int i;

	if (argc < 2)
		sysfatal("usage: bf9 <source file>");
	load(argv[1]);
	for(i=0;i <MEMSIZ; i++)
		memory[i]=0;
	run(program);
	exits(nil);
}