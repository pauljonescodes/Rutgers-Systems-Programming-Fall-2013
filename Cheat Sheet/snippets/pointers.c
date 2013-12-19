/* wrong */ void swap(int x, int y) {
int temp; temp = x; x = y; y = temp;}
/* right */void swap(int*px, int*py){
int temp;temp=*px;*px=*py; *py=temp;}
