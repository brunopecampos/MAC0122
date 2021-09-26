int *jump = malloc ((tPal+1) * sizeof (int));
    // usaremos jump[1..m]

   // pré-processamento da palavra a
   int h = tPal, k = tPal-1;
   while (h >= 1 && k >= 1) {
      int i = tPal, j = k; 
      while (i >= h && j >= 1)
         if (pal[i] == pal[j]) --i, --j;
         else i = tPal, j = --k;
      jump[h--] = k;
   }
   while (h >= 1)
      jump[h--] = k;

   // busca da palavra a no texto b
   int ocorrs = 0;
   k = tPal;
   while (k <= tTex) {
      int i = tPal, j = k;
      while (i >= 1 && pal[i] == texto[j]) 
         --i, --j;   
      if (i < 1) ++ocorrs;
      if (i == tPal) k += 1;
      else k += tPal - jump[i+1];
   }
   return ocorrs;