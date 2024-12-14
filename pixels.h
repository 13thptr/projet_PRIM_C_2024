/*our accéder à la composante k∈[0⋯c−1] d’un pixel situé à la ligne i∈[0⋯h−1] 
et à la colonne j∈[0⋯w−1] d’une image couleur p on pourra donc utiliser ...data[((i * w + j) * c) + k].
Vous pourrez isoler les opérations liées à la lecture ou à l’écriture d’une composante de pixel (i,j,c)

dans une image dans un sous module pixels.[h/c] dans lequel pour pourrez aussi avantageusement
 définir des constantes symboliques (en utilisant un enum par exemple) RED, GREEN et BLUE pour accéder directement à ces composantes dans les pixels.
*/
enum PIXEL_TYPES{RED,GREEN,BLUE};
