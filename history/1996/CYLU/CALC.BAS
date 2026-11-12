DIM nInput%
DO
   INPUT nInput%
   PRINT "nInput%:"; nInput%; "Answer:"; (INT(nInput% / 7) * 7);
LOOP UNTIL nInput% = -1

