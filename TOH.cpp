#include <iostream>

char Pole(int p)
{
   switch(p)
   {
      case 0:
         return 'A';
      case 1:
         return 'C';
      case 2:
         return 'B';
   }

   return '\0';
}


void Relocate(int disk, int pole0, int pole1, int polei)
{
   if(disk > 0)
      Relocate(disk - 1, pole0, polei, pole1);

   std::cout << "moving disk " << disk << " from pole " << Pole(pole0) << " to pole " << Pole(pole1) << std::endl;

   if(disk > 0)
      Relocate(disk - 1, polei, pole1, pole0);
}


void TOH(int disks)
{
   if(disks > 0)
   {
      std::cout << "moving " << disks << " disks...\n" << std::endl;

      Relocate(disks - 1, 0, 2, 1);
   }
}


// 17th Jan 2012
// took 45-50 minutes
int main()
{
   TOH(1);
}
