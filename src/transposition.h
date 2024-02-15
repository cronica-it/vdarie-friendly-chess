
int variante[100000][6];
int id;
int departarea;
int contor_variante ;
int DepuneVarianta(int ponderea,int indicatori);
int RegasireVarianta(int caz);
int apel_depuneri;
int gasit_depuneri;

int m0, m1, m2, m3;

int ClearVariante()
{
    
	for(id=0;id < 100000;id++)
	{
		variante[id][0] = 0;
		variante[id][1] = 0;	
		variante[id][2] = 0;
		variante[id][3] = 0;
		variante[id][4] = 0;
		variante[id][5] = 0;		
	}	
	// depune o noua varianta; 
	
	contor_variante = 0;
	
	return 1;
}

int DepuneVarianta(int ponderea,int indicatori)
{
	departarea = gameDepth - treeDepth;
	if(departarea != 3) return 0;
if(Stack2[treeDepth].IsCheckOwn != 0 || Stack2[treeDepth + 1].IsCheckOwn != 0 || 
	Stack2[treeDepth + 2].IsCheckOwn != 0 || Stack2[treeDepth + 3].IsCheckOwn != 0 ) return 0;
if(Stack2[treeDepth].IsCheck != 0 || Stack2[treeDepth + 1].IsCheck != 0 || 
	Stack2[treeDepth + 2].IsCheck != 0 || Stack2[treeDepth + 3].IsCheck != 0 ) return 0;

	m0 = Stack2[treeDepth].move;
	m1 = Stack2[treeDepth + 1].move;
	m2 = Stack2[treeDepth + 2].move;
	m3 = Stack2[treeDepth + 3].move;	
	
	for(id=0;id < contor_variante;id++)
	{
		if(m0 != variante[id][0]) continue;
		if(m1 != variante[id][1]) continue;
		if(m2 != variante[id][2]) continue;	
		if(m3 != variante[id][3]) continue;			
		// gasit varianta; daca indicatori == 1 return cai protejam variante analizate complet
		if(indicatori != 1 ) return 0;

		variante[id][4] = ponderea;
		variante[id][5] = indicatori;	
		
		return 2; // este deja
	}	
	// depune o noua varianta; 
	
	if(contor_variante >= 100000) return 0;
	
	variante[contor_variante][0] = m0;
	variante[contor_variante][1] = m1;
	variante[contor_variante][2] = m2;
	variante[contor_variante][3] = m3;	
	variante[contor_variante][4] = ponderea;
	variante[contor_variante][5] = indicatori;	
	contor_variante++;	
	return 1;
}  
int RegasireVarianta(int caz)
{
// return 0;
	departarea = gameDepth - treeDepth;
	if(departarea != 3) return 0;

if(Stack2[treeDepth].IsCheckOwn != 0 || Stack2[treeDepth + 1].IsCheckOwn != 0 || 
	Stack2[treeDepth + 2].IsCheckOwn != 0 || Stack2[treeDepth + 3].IsCheckOwn != 0 ) return 0;
if(Stack2[treeDepth].IsCheck != 0 || Stack2[treeDepth + 1].IsCheck != 0 || 
	Stack2[treeDepth + 2].IsCheck != 0 || Stack2[treeDepth + 3].IsCheck != 0 ) return 0;
	
int v0,v1, v2,v3;
	
	m0 = Stack2[treeDepth].move;
	m1 = Stack2[treeDepth + 1].move;
	m2 = Stack2[treeDepth + 2].move;
	m3 = Stack2[treeDepth + 3].move;   

	
	for(id=0;id < contor_variante;id++)
	{
		v0 = variante[id][0];
		v1 = variante[id][1];
		v2 = variante[id][2];
		v3 = variante[id][3];
		if(((m0 == v0) && (m2 == v2) || (m0 == v2) && (m2 == v0)) && 
			((m1 == v1) && (m3 == v3) || (m1 == v3) && (m3 == v1)))
			break; // gasit varianta
	}	
	// depune o noua varianta; daca exista deja, vechea stocare are prioritate
	if(id >= contor_variante) return 0;

	regasiri_variante++;

	pondere_return = variante[id][4] ;
	indi_return = variante[id][5] ; //  indicatori daca e plin sau taiere de arbore
	return 0;
	return 1;
}
