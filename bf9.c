#include <u.h>
#include <libc.h>
#include <bio.h>

#define MEMSIZ 65535

char memory[MEMSIZ];
char *program;

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
	ulong nr;
	vlong siz;

	if((buf = Bopen(path, OREAD)) == nil)
		sysfatal("Bopen: %r");
	siz = Bseek(buf, 0L, 2);
	Bseek(buf, 0L, 0);
	if((program = malloc(sizeof(*program)*siz)) == nil)
		sysfatal("malloc: %r");
	if((nr = Bread(buf, program, siz)) != siz)
		sysfatal("Bread: %r");
	Bterm(buf);
	USED(nr);
}

void
main(int argc, char* argv[])
{
	if(argc < 2){
		fprint(2, "usage: %s <source file>\n", argv0);
		exits("usage");
	}
	load(argv[1]);
	memset(memory, 0, sizeof memory);
	run(program);
	exits(nil);
}
