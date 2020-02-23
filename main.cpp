#include <iostream>

template <typename Tip>
class Lista {
    public:
    Lista() {}
    virtual ~Lista() {}
    virtual int brojElemenata() const = 0;
    virtual Tip& trenutni() = 0;
    virtual bool prethodni() = 0;
    virtual bool sljedeci() = 0;
    virtual void pocetak() = 0;
    virtual void kraj() = 0;
    virtual void obrisi() = 0;
    virtual void dodajIspred(const Tip &el) = 0;
    virtual void dodajIza(const Tip &el) = 0;
    virtual Tip& operator[](int i) = 0;
};

template <typename Tip>
class DvostrukaLista : public Lista<Tip> {
    struct Cvor {
        Tip element;
        Cvor *prethodni;
        Cvor *sljedeci;
        Cvor(const Tip &el) {
            element=el;
            prethodni=nullptr;
            sljedeci=nullptr;
        }
    };
    Cvor *pPocetak, *pKraj, *pTrenutni;
    int duzina;
    public:
    DvostrukaLista() : pPocetak(nullptr), pKraj(nullptr), pTrenutni(nullptr), duzina(0) {}
    ~DvostrukaLista() { Unisti(); }
    DvostrukaLista(const DvostrukaLista &dl);
    DvostrukaLista& operator =(const DvostrukaLista &dl);
    int brojElemenata() const { return duzina; }
    Tip& trenutni() { 
        if (duzina == 0) throw "Lista prazna"; 
        return pTrenutni->element; 
    }
    Tip trenutni() const { 
        if (duzina == 0) throw "Lista prazna"; 
        return pTrenutni->element; 
    }
    bool prethodni() {
        if (duzina == 0) throw "Lista prazna";
        if (pTrenutni == pPocetak) return false;
        pTrenutni = pTrenutni->prethodni;
        return true;
    }
    bool sljedeci() {
        if (duzina == 0) throw "Lista prazna";
        if (pTrenutni == pKraj) return false;
        pTrenutni = pTrenutni->sljedeci;
        return true;
    }
    void pocetak() { pTrenutni = pPocetak; }
    void kraj() { pTrenutni = pKraj; }
    void obrisi();
    void dodajIspred(const Tip &el);
    void dodajIza(const Tip &el);
    Tip& operator[](int i);
    Tip operator[](int i) const;
    void Unisti();
};


template <typename Tip>
class DvostraniRed {
    DvostrukaLista<Tip> dek;
    public:
    DvostraniRed() : dek() {}
    ~DvostraniRed() { dek.Unisti(); }
    void brisi() { dek.Unisti(); }
    int brojElemenata() { return dek.brojElemenata(); }
    void staviNaVrh(const Tip &el) { dek.kraj(); dek.dodajIza(el); }
    Tip skiniSaVrha() { 
        dek.kraj(); 
        Tip pomocni= dek.trenutni();
        dek.obrisi();
        return pomocni; 
    }
    void staviNaCelo(const Tip &el) { dek.pocetak(); dek.dodajIspred(el); }
    Tip skiniSaCela() { 
        dek.pocetak(); 
        Tip pomocni= dek.trenutni();
        dek.obrisi(); 
        return pomocni; 
        
    }
    Tip& vrh() { 
        if (dek.brojElemenata() == 0) throw "Dek prazan";
        dek.kraj(); return dek.trenutni();
    }
    Tip& celo() { 
        if (dek.brojElemenata() == 0) throw "Dek prazan";
        dek.pocetak(); return dek.trenutni();
    }
    
};


template <typename Tip>
void DvostrukaLista<Tip>::Unisti() {
    Cvor *p = pPocetak;
    while (p != nullptr) {
        Cvor *p2 = p->sljedeci;
        delete p;
        p = p2;
    }
    pPocetak = pKraj = pTrenutni = nullptr; duzina = 0;
}

template <typename Tip>
DvostrukaLista<Tip>::DvostrukaLista(const DvostrukaLista &dl) {
    duzina = dl.duzina;
    Cvor *pdl = dl.pPocetak; 
    Cvor *p = new Cvor(pdl->element);
    pdl = pdl->sljedeci; pPocetak = p;
    while (pdl != nullptr) {
        p->sljedeci = new Cvor(pdl->element);
        p->sljedeci->prethodni = p;
        pdl = pdl->sljedeci; p = p->sljedeci;
    }
    pKraj = p;
    pdl = dl.pPocetak; p = pPocetak;
    while (pdl != dl.pTrenutni) { pdl = pdl->sljedeci; p = p->sljedeci; }
    pTrenutni = p;
}

template <typename Tip>
DvostrukaLista<Tip>& DvostrukaLista<Tip>::operator =(const DvostrukaLista &dl) {
    if (&dl == this) return *this;
    Cvor *p = pPocetak;
    while (p != nullptr) {
        Cvor *p2 = p->sljedeci;
        delete p;
        p = p2;
    }
    duzina = dl.duzina;
    Cvor *pdl = dl.pPocetak; 
    p = new Cvor(pdl->element);
    pdl = pdl->sljedeci; pPocetak = p;
    while (pdl != nullptr) {
        p->sljedeci = new Cvor(pdl->element);
        p->sljedeci->prethodni = p;
        pdl = pdl->sljedeci; p = p->sljedeci;
    }
    pKraj = p;
    pdl = dl.pPocetak; p = pPocetak;
    while (pdl != dl.pTrenutni) { pdl = pdl->sljedeci; p = p->sljedeci; }
    pTrenutni = p;
    return *this;
}

template <typename Tip>
void DvostrukaLista<Tip>::dodajIspred(const Tip &el) {
    if (pTrenutni == pPocetak) {
        Cvor *novi = new Cvor(el);
        novi->prethodni = nullptr;
        if (pPocetak != nullptr) { 
            pPocetak->prethodni = novi;
            novi->sljedeci = pPocetak;
        }
        pPocetak = novi;
        if (duzina == 0) {
            pKraj = novi;
            pTrenutni = novi;
        }
    }
    else {
        Cvor *novi = new Cvor(el);
        novi->prethodni = pTrenutni->prethodni;
        novi->sljedeci = pTrenutni;
        pTrenutni->prethodni->sljedeci = novi;
        pTrenutni->prethodni = novi;
        
    }
    duzina++;
}

template <typename Tip>
void DvostrukaLista<Tip>::dodajIza(const Tip &el) {
    if (pTrenutni == pKraj) {
        Cvor *novi = new Cvor(el);
        novi->sljedeci = nullptr;
        if (duzina == 0) {
            novi->prethodni = nullptr;
            pPocetak = novi;
            pTrenutni = novi;
        }
        else {
            pTrenutni->sljedeci = novi;
            novi->prethodni = pTrenutni;
        }
        pKraj = novi;
    }
    else {
        Cvor *novi = new Cvor(el);
        novi->prethodni = pTrenutni;
        novi->sljedeci = pTrenutni->sljedeci;
        pTrenutni->sljedeci = novi;
        pTrenutni->sljedeci->prethodni = novi;
    }
    duzina++;
}

template <typename Tip>
void DvostrukaLista<Tip>::obrisi() {
    if (duzina == 0) throw "Lista prazna";
    if (duzina == 1) {
        delete pTrenutni;
        pPocetak = pKraj = pTrenutni = nullptr;
    }
    else if (pTrenutni == pPocetak) {
        pPocetak = pTrenutni->sljedeci;
        pPocetak->prethodni = nullptr;
        delete pTrenutni;
        pTrenutni = pPocetak;
    }
    else if (pTrenutni == pKraj) {
        pKraj = pTrenutni->prethodni;
        pKraj->sljedeci = nullptr;
        delete pTrenutni;
        pTrenutni = pKraj;
    }
    else {
        pTrenutni->prethodni->sljedeci = pTrenutni->sljedeci;
        pTrenutni->sljedeci->prethodni = pTrenutni->prethodni;
        Cvor *p = pTrenutni->sljedeci;  
        delete pTrenutni;
        pTrenutni =p;
    }
    duzina--;
}

template <typename Tip>
Tip& DvostrukaLista<Tip>::operator[](int i) {
    if (duzina == 0) throw "Lista prazna";
    if (i<0 || i>=duzina) throw "Nepravilan indeks";
    Cvor *p = pPocetak; int brojac=0;
    while (brojac < i) {
        p = p->sljedeci;
        brojac++;   
    }
    return p->element;
}

template <typename Tip>
Tip DvostrukaLista<Tip>::operator[](int i) const {
    if (duzina == 0) throw "Lista prazna";
    if (i<0 || i>=duzina) throw "Nepravilan indeks";
    Cvor *p = pPocetak; int brojac=0;
    while (brojac < i) {
        p = p->sljedeci;
        brojac++;   
    }
    return p->element;
}


int main() {
    return 0;
}
