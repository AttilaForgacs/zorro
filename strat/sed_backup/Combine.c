//script for merging all WFO .par and .fac files
//from two strategies to build a combined strategy 

string src1 = "Z1";
string src2 = "Z2"; // must be different to dest
string dest = "Z12";

int file_merge(int n,string ext)
{
	char name1[40],name2[40],name3[40];
	if(n) {
		sprintf(name1,"Data\\%s_%i.%s",src1,n,ext);
		sprintf(name2,"Data\\%s_%i.%s",src2,n,ext);
		sprintf(name3,"Data\\%s_%i.%s",dest,n,ext);
	} else {
		sprintf(name1,"Data\\%s.%s",src1,ext);
		sprintf(name2,"Data\\%s.%s",src2,ext);
		sprintf(name3,"Data\\%s.%s",dest,ext);	
	}
	if(!file_date(name1)) return 0;
	if(0 != strcmp(name3,name1))
		if(!file_copy(name3,name1)) return 0;
	if(!file_append(name3,file_content(name2))) return 0;
	return 1;
}


function main()
{
	int cycles = 1;
	for(; cycles < 100; cycles++)
		if(!file_merge(cycles,"par")) break;
	
	cycles += file_merge(0,"par");
	cycles += file_merge(0,"fac");
	
	if(cycles > 3) 
		printf("%i files combined!",cycles);
	else
		printf("Error!");
}