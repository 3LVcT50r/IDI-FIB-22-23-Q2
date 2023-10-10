#version 330 core

in vec3  fmatamb;
in vec3  fmatdiff;
in vec3  fmatspec;
in float fmatshin;
in vec3  fvertex;
in vec3  fnormal;

const vec3 llumAmbient = vec3(0.1, 0.1, 0.1);

uniform mat4 TGB;
uniform mat4 Proj;
uniform mat4 View;

uniform vec3 posFocusI;
uniform vec3 posFocus0;
uniform vec3 posFocus1;
uniform vec3 posFocus2;
uniform vec3 posFocus3;
uniform vec3 colorFocusI;
uniform vec3 colorFocus0;
uniform vec3 colorFocus1;
uniform vec3 colorFocus2;
uniform vec3 colorFocus3;

out vec4 FragColor;


vec3 Ambient() {
  return llumAmbient*fmatamb;
}

vec3 Difus (vec3 NormSCO, vec3 L, vec3 colFocus)
{
  // Fixeu-vos que SOLS es retorna el terme de Lambert!
  // S'assumeix que els vectors que es reben com a paràmetres estan normalitzats
  vec3 colRes = vec3(0);
  if (dot (L, NormSCO) > 0)
    colRes = colFocus * fmatdiff * dot (L, NormSCO);
  return (colRes);
}

vec3 Especular (vec3 NormSCO, vec3 L, vec3 vertSCO, vec3 colFocus)
{
  // Fixeu-vos que SOLS es retorna el terme especular!
  // Assumim que els vectors estan normalitzats
  vec3 colRes = vec3 (0);
  // Si la llum ve de darrera o el material és mate no fem res
  if ((dot(NormSCO,L) < 0) || (fmatshin == 0))
    return colRes;  // no hi ha component especular

  vec3 R = reflect(-L, NormSCO); // equival a: 2.0*dot(NormSCO,L)*NormSCO - L;
  vec3 V = normalize(-vertSCO); // perquè la càmera està a (0,0,0) en SCO

  if (dot(R, V) < 0)
    return colRes;  // no hi ha component especular

  float shine = pow(max(0.0, dot(R, V)), fmatshin);
  return (colRes + fmatspec * colFocus * shine);
}

void main()
{
  //fcolor = fmatdiff;
  vec3 normal = normalize(fnormal);
  
  vec3 fposFocusI = (View * vec4(posFocusI, 1.0)).xyz;
  vec3 LI = normalize(fposFocusI - fvertex);

  vec3 fposFocus0 = (View * TGB * vec4(posFocus0, 1.0)).xyz;
  vec3 L0 = normalize(fposFocus0 - fvertex);

  vec3 fposFocus1 = (View * TGB * vec4(posFocus1, 1.0)).xyz;
  vec3 L1 = normalize(fposFocus1 - fvertex);

  vec3 fposFocus2 = (View * TGB * vec4(posFocus2, 1.0)).xyz;
  vec3 L2 = normalize(fposFocus2 - fvertex);

  vec3 fposFocus3 = (View * TGB * vec4(posFocus3, 1.0)).xyz;
  vec3 L3 = normalize(fposFocus3 - fvertex);

  vec3 fcolorI = Difus(normal, LI, colorFocusI) + Especular(normal, LI, fvertex, colorFocusI);
  vec3 fcolor0 = Difus(normal, L0, colorFocus0) + Especular(normal, L0, fvertex, colorFocus0);
  vec3 fcolor1 = Difus(normal, L1, colorFocus1) + Especular(normal, L1, fvertex, colorFocus1);
  vec3 fcolor2 = Difus(normal, L2, colorFocus2) + Especular(normal, L2, fvertex, colorFocus2);
  vec3 fcolor3 = Difus(normal, L3, colorFocus3) + Especular(normal, L3, fvertex, colorFocus3);


  vec3 fcolor = Ambient()+ fcolorI + fcolor0 + fcolor1 + fcolor2 + fcolor3;
  FragColor = vec4(fcolor, 1);
}
