"""Questao 2: S-DES didatico para blocos de 8 bits."""
P10=[3,5,2,7,4,10,1,9,8,6]
P8=[6,3,7,4,8,5,10,9]
IP8=[2,6,3,1,4,8,5,7]
IP8_INV=[4,1,3,5,7,2,8,6]
EP=[4,1,2,3,2,3,4,1]
P4=[2,4,3,1]
S0=[[1,0,3,2],[3,2,1,0],[0,2,1,3],[3,1,3,2]]
S1=[[0,1,2,3],[2,0,1,3],[3,0,1,0],[2,1,0,3]]

def perm(bits, table): return ''.join(bits[i-1] for i in table)
def xor(a,b): return ''.join(str(int(x)^int(y)) for x,y in zip(a,b))
def rot(s,n): return s[n:]+s[:n]
def s_lookup(b,s): return f'{s[int(b[0]+b[3],2)][int(b[1:3],2)]:02b}'
def sdes_keys(key, verbose=False):
    p=perm(key,P10); a,b=p[:5],p[5:]; a,b=rot(a,1),rot(b,1); k1=perm(a+b,P8)
    if verbose: print('P10',p,'LS1',a+b,'K1',k1)
    a,b=rot(a,2),rot(b,2); k2=perm(a+b,P8)
    if verbose: print('LS2',a+b,'K2',k2)
    return k1,k2

def sdes_round(halfL,halfR,key,verbose=False):
    e=perm(halfR,EP); x=xor(e,key); sub=s_lookup(x[:4],S0)+s_lookup(x[4:],S1); p=perm(sub,P4); l=xor(halfL,p)
    if verbose: print('E/P',e,'XOR',x,'S0|S1',sub,'P4',p,'LxorP4',l,'R',halfR)
    return l,halfR

def sdes(bits,key,decrypt=False,verbose=False):
    ks=sdes_keys(key,verbose)
    if decrypt: ks=ks[::-1]
    p=perm(bits,IP8); l,r=p[:4],p[4:]
    if verbose: print('IP',p)
    l,r=sdes_round(l,r,ks[0],verbose)
    l,r=r,l
    if verbose: print('SW',l+r)
    l,r=sdes_round(l,r,ks[1],verbose)
    if verbose: print('preoutput',l+r)
    return perm(l+r,IP8_INV)


if __name__ == '__main__':
    mensagem = '01111001'
    chave = '1001011101'
    print('Questao 2 - S-DES')
    print('Mensagem:', mensagem, 'Chave:', chave)
    cifrada = sdes(mensagem, chave, verbose=True)
    recuperada = sdes(cifrada, chave, decrypt=True)
    print('Criptografada:', cifrada)
    print('Descriptografada:', recuperada)
    assert recuperada == mensagem
