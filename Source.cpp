#include<iostream>

struct Imobil {
	unsigned int idImobil;
	char* numeStrada;
	int nrAp;
	int* nrLoc;
	int valTotala;
};


struct nod
{
	Imobil info;
	nod* st;
	nod* dr;
};

nod* creareNod(Imobil info, nod* st, nod*dr)
{
	nod* nou = (nod*)malloc(sizeof(nod));
	nou->info.idImobil = info.idImobil;
	nou->info.numeStrada = (char*)malloc(strlen(info.numeStrada) + 1);
	strcpy(nou->info.numeStrada, info.numeStrada);
	nou->info.nrAp = info.nrAp;
	nou->info.nrLoc = (int*)malloc(info.nrAp*sizeof(int));
	for (int i = 0; i < info.nrAp; i++)
		nou->info.nrLoc[i] = info.nrLoc[i];
	nou->info.valTotala = info.valTotala;

	nou->st = st;
	nou->dr = dr;
	return nou;
}

//nr nivele
int max(int a, int b)
{
	if (a > b) return a;
	else return b;
}

int inaltime(nod* root)
{
	if (root)
	{
		return(max(inaltime(root->st), inaltime(root->dr)) + 1);
	}
	else
		return 0;
}

//factor balansare
int factor(nod* root)
{
	int hSt = inaltime(root->st);
	int hDr = inaltime(root->dr);
	return (hSt - hDr);
}

//rotatie dreapta
nod* rotatieDreapta(nod* root)
{
	nod* aux = root->st;
	root->st = root->dr;
	root->dr = aux;
	return aux;
}

//rotatie stanga
nod* rotatieStanga(nod* root)
{
	nod* aux = root->dr;
	root->dr = root->st;
	root->st = aux;
	return aux;
}

nod* inserare(nod* root, Imobil info)
{
	if (root == NULL)
	{
		root = creareNod(info, NULL, NULL);
		return root;
	}
	else
	{
		if (info.idImobil < root->info.idImobil)
		{
			root->st = inserare(root->st, info);
		}
		else
		{
			root->dr = inserare(root->dr, info);
		}
		//verificare factor balans
		if (factor(root) == 2)
		{
			if (factor(root->st) == 1)
			{
				root = rotatieDreapta(root);
			}
			if (factor(root->st) == -1)
			{
				root->st = rotatieStanga(root->st);
				root = rotatieDreapta(root);
			}
		}
		if (factor(root) == -2)
		{
			if (factor(root->dr) == -1)
			{
				root = rotatieStanga(root);
			}
			if (factor(root->dr) == 1)
			{
				root->dr = rotatieDreapta(root->dr);
				root = rotatieStanga(root);
			}
		}
		return root;
	}
}

void afisareIn(nod* root)
{
	if (root)
	{
		afisareIn(root->st);
		printf("\n %d %s %d", root->info.idImobil, root->info.numeStrada, root->info.nrAp);
		for (int i = 0; i < root->info.nrAp; i++)
			printf(" %d,", root->info.nrLoc[i]);
		printf("Val: %d", root->info.valTotala);
		afisareIn(root->dr);
	}
}

//GRESITA! - merge doar daca dau ca paramentru numele strazii primului obiect din fisieer
int nrimobilPeStrada(nod* root, char* strada)
{
	if (root && strcmp(root->info.numeStrada, strada) == 0)
	{
		return 1 + nrimobilPeStrada(root->st, strada) + nrimobilPeStrada(root->dr, strada);
	}
	else
		return 0;
}


//GRESITA! - merge doar daca dau ca paramentru id-ul primului obiect din fisieer
int calculPlataPersoana(nod* root, int id)
{
	int sum = 0;
	if (root && root->info.idImobil == id)
	{
		for (int i = 0; i < root->info.nrAp; i++)
			sum = sum + root->info.nrLoc[i];
		return (root->info.valTotala / sum) + calculPlataPersoana(root->st, id) + calculPlataPersoana(root->dr, id);
	}
	else
		return 0;
}



nod* citireFisier(char* numeF)
{
	FILE* f = fopen(numeF, "r");
	Imobil i;
	nod* root=NULL;
	int dim;
	char buffer[100];
	char* token[20];
	char vector[20];
	while (!feof(f))
	{
		
		fscanf(f, "%d", &i.idImobil);
		fscanf(f, "%s", buffer);
		i.numeStrada = (char*)malloc(strlen(buffer) + 1);
		strcpy(i.numeStrada, buffer);
		fscanf(f, "%d", &i.nrAp);
		dim = i.nrAp;
		i.nrLoc = (int*)malloc(dim*sizeof(int));
		fscanf(f, "%s", vector);
		token[0] = strtok(vector, ",");
		for (int i = 1; i < dim; i++)
		{
			token[i] = strtok(NULL, ",");
		}
		for (int j = 0; j < dim; j++)
		{
			i.nrLoc[j] = atoi(token[j]);
		}

		fscanf(f, "%d", &i.valTotala);

		root = inserare(root, i);
		
	}
	return root;
}

void main()
{
	nod* root=citireFisier("Text.txt");
	afisareIn(root);
	int nr = nrimobilPeStrada(root, "florilor");
	printf("\n Nr imobile: %d", nr);
	int sumaPlataa = calculPlataPersoana(root, 3);
	printf("\n De platit: %d", sumaPlataa);
}