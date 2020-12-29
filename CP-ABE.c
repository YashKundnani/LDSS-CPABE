#include <stdio.h>
#include<stdlib.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>


//ECC does not directly provide encryption like RSA does
// so using ECDH Ellicptical curve Diffe Hillman 
struct EquationOfCurve{
    //y^2 = x^3 + ax +7
    double a;
    double b;
    
};



struct TwoD_Point{
    long double x;
    long double y;
};

struct EncryptKey{
    struct TwoD_Point sharedECCKey;
    struct TwoD_Point ciphertextPubKey; 
};


    
struct TwoD_Point Calculate_PublicKey(struct TwoD_Point P, long unsigned int SK)
{//X=x.P retuens X(PK) with using x(SK); SK = Secret Key and PK = public key
    struct TwoD_Point PK;
    PK.x = SK*P.x;
    PK.y = SK*P.y;
    
    return PK;
    
    
}

long int Calculate_Y (long int x,struct EquationOfCurve EqCurve){
    long double y;
    //calculate y wrt x using equation of curve 
    y = sqrt((EqCurve.a*(x^3))+EqCurve.b);
    
    return y;
}


bool checkPrimeNumber(long unsigned int num){

	{
	if(num%2==0)
		return false;
	else
		for(int i=3;i<=sqrt(num);i=i+2)
		{
			if(num%i==0)
				return false;
			
		}
		return true;
	}


}

long unsigned int generatePrimeNumber(int seed){
	int x = seed;
	while(1)
	{
		x = x+7;
		srand(time(NULL)+x);
		long unsigned int num = rand();
		if(checkPrimeNumber(num))
			return num;
		else 
			continue;
	}
}

struct TwoD_Point generateP (struct EquationOfCurve EqCurve){
    struct TwoD_Point P;
    srand(time(NULL)); // to seed random value 
    P.x = rand(); 
    if(P.x < 0){
        P.x = -(P.x);
        //for every positive value of x there exits a y
    }
    P.y = Calculate_Y(P.x,EqCurve);
    printf("Master Key : \n (%Lf, %Lf)\n", P.x, P.y);
    return P;
}


struct EncryptKey calculateEncryptKey(struct TwoD_Point X,struct TwoD_Point P){
    //P is used as Generator and X is the public Key;
    srand(time(NULL));
    int r = rand();
    
    long unsigned int ciphertextPrivKey = generatePrimeNumber(r);//encrypting
    printf(" Data : %ld\n", ciphertextPrivKey);
    printf(" Encrypting Data ... \n");
    struct TwoD_Point ciphertextPubKey;
    ciphertextPubKey.x = ciphertextPrivKey*P.x;
    ciphertextPubKey.y = ciphertextPrivKey*P.y;
    
    struct TwoD_Point sharedECCKey;
    sharedECCKey.x = X.x * ciphertextPrivKey;// transferring Key
    sharedECCKey.y = X.y * ciphertextPrivKey;
    
    struct EncryptKey e;
    e.sharedECCKey =sharedECCKey;
    e.ciphertextPubKey = ciphertextPubKey;
    printf(" Encrypted Data to Transfer: \n (%Lf, %Lf)\n", e.ciphertextPubKey.x, e.ciphertextPubKey.y);
    
    return e;// contains sharedECCKey and ciphertextPubKey
    //encrypted using sharedECCKey
}

struct TwoD_Point calculateDecryptKey(long unsigned int SK,struct TwoD_Point X, struct TwoD_Point ciphertextPubKey){
    struct TwoD_Point decryptKey;// same as sharedECCKey
    printf("Received Data:\n (%Lf, %Lf)\n", ciphertextPubKey.x, ciphertextPubKey.y);
    decryptKey.x = ciphertextPubKey.x * SK;
    decryptKey.y = ciphertextPubKey.y * SK;
    printf("Decrypting Data...\n");
    printf("Data: \n");
    
    printf("%Lf \n%Lf \n", (decryptKey.x)/(X.x), (decryptKey.y)/(X.y) );
    return decryptKey; // sharedECCKey = decryptKey
}

struct MasterKey {
	long unsigned int a;
	long unsigned int b;
};

struct PrivateKey{
   struct EquationOfCurve EqCurve;
    struct TwoD_Point g;
    struct TwoD_Point gRaisedTob; //g^b
    struct TwoD_Point bilinearMappedg; //e(g,g)^a = (g+3g)^b
    int attributeArray[3]; // 0 for Occupation; 1 for Memebership and 2 for LivingIn
};
struct OwnerKey{
    struct MasterKey mk;
    struct PrivateKey pk;
    
};

struct Attributes{
    
    int Occupation;  // 0-Agriculture;1-Pottery;2-Fishery;3-Scientest
    int Membership; // 0-Free; 1-Paid
    int LivingIn;  // 0- Hills; 1-Desert; 2-Coast; 3-Plains
    
};


struct OwnerKey GenerateMasterKey(struct Attributes A){
    
    struct OwnerKey master_key;
    return master_key;
}


int main()
{
	srand(time(NULL));
	int r = rand();
    long unsigned int SK = generatePrimeNumber(r); // Secret key in   X=x.P (X=PK,x=SK)
    struct EquationOfCurve EqCurve;
    EqCurve.a = 1;
    EqCurve.b = 7;
    printf(" Making Master Key...\n");
    struct TwoD_Point P = generateP(EqCurve); // a random point on the curve, used as Generator
    //P may satisfy properties of bilinear pairing
    
    struct TwoD_Point publicKey = Calculate_PublicKey(P,SK); // public key
    struct EncryptKey encrypt_key = calculateEncryptKey(publicKey,P);
    struct TwoD_Point decrypt_Key = calculateDecryptKey(SK, publicKey, encrypt_key.ciphertextPubKey); 
    
    // printf("sharedECCKey Calculated by Encrypt Calculator: x=%Lf, y=%Lf",encrypt_key.sharedECCKey.x,encrypt_key.sharedECCKey.y);
    // printf("\nsharedECCKey Calculated by Decrypt Calculator: x=%Lf, y=%Lf",decrypt_Key.x,decrypt_Key.y);
    
    
}


