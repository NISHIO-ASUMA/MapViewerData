xof 0302txt 0064
template Header {
 <3D82AB43-62DA-11cf-AB39-0020AF71E433>
 WORD major;
 WORD minor;
 DWORD flags;
}

template Vector {
 <3D82AB5E-62DA-11cf-AB39-0020AF71E433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template Coords2d {
 <F6F23F44-7686-11cf-8F52-0040333594A3>
 FLOAT u;
 FLOAT v;
}

template Matrix4x4 {
 <F6F23F45-7686-11cf-8F52-0040333594A3>
 array FLOAT matrix[16];
}

template ColorRGBA {
 <35FF44E0-6C7C-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <D3E16E81-7835-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template IndexedColor {
 <1630B820-7842-11cf-8F52-0040333594A3>
 DWORD index;
 ColorRGBA indexColor;
}

template Boolean {
 <4885AE61-78E8-11cf-8F52-0040333594A3>
 WORD truefalse;
}

template Boolean2d {
 <4885AE63-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template MaterialWrap {
 <4885AE60-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template TextureFilename {
 <A42790E1-7810-11cf-8F52-0040333594A3>
 STRING filename;
}

template Material {
 <3D82AB4D-62DA-11cf-AB39-0020AF71E433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template MeshFace {
 <3D82AB5F-62DA-11cf-AB39-0020AF71E433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template MeshFaceWraps {
 <4885AE62-78E8-11cf-8F52-0040333594A3>
 DWORD nFaceWrapValues;
 Boolean2d faceWrapValues;
}

template MeshTextureCoords {
 <F6F23F40-7686-11cf-8F52-0040333594A3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}

template MeshMaterialList {
 <F6F23F42-7686-11cf-8F52-0040333594A3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material]
}

template MeshNormals {
 <F6F23F43-7686-11cf-8F52-0040333594A3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template MeshVertexColors {
 <1630B821-7842-11cf-8F52-0040333594A3>
 DWORD nVertexColors;
 array IndexedColor vertexColors[nVertexColors];
}

template Mesh {
 <3D82AB44-62DA-11cf-AB39-0020AF71E433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

Header{
1;
0;
1;
}

Mesh {
 28;
 55.06105;137.66802;23.91102;,
 55.06101;105.37744;-32.01805;,
 -331.90271;105.37744;-32.01805;,
 -331.90271;137.66802;23.91102;,
 53.73432;40.79595;-32.01805;,
 -331.90271;40.79595;-32.01805;,
 55.06101;8.50552;23.91102;,
 -331.90276;8.50552;23.91102;,
 55.29588;8.08276;23.91102;,
 52.35704;40.25486;-32.67261;,
 233.90393;108.03819;-32.67261;,
 250.23853;79.74619;23.91102;,
 54.07288;105.28621;-32.67261;,
 201.23622;164.62193;-32.67261;,
 54.42592;139.58836;23.91102;,
 184.90166;192.91375;23.91102;,
 333.69078;246.60225;-32.67261;,
 359.45406;225.60315;23.91102;,
 276.48875;282.28733;-32.67261;,
 251.46265;309.59913;23.91102;,
 350.31419;437.43550;-32.67261;,
 382.62366;432.60660;23.91102;,
 285.69522;447.09273;-32.67261;,
 253.38511;451.92134;23.91102;,
 344.67278;616.63004;-32.67261;,
 376.68519;613.10709;23.91102;,
 280.64676;615.47037;-32.67261;,
 248.63306;608.95728;23.91102;;
 
 15;
 4;0,1,2,3;,
 4;1,4,5,2;,
 4;4,6,7,5;,
 4;8,9,10,11;,
 4;9,12,13,10;,
 4;12,14,15,13;,
 4;11,10,16,17;,
 4;10,13,18,16;,
 4;13,15,19,18;,
 4;17,16,20,21;,
 4;16,18,22,20;,
 4;18,19,23,22;,
 4;21,20,24,25;,
 4;20,22,26,24;,
 4;22,23,27,26;;
 
 MeshMaterialList {
  1;
  15;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0;;
  Material {
   0.599200;0.467200;0.000000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  13;
  0.000000;0.866026;-0.499999;,
  0.000000;0.000000;-1.000000;,
  0.000000;-0.866027;-0.499997;,
  -0.868906;-0.026173;-0.494284;,
  0.869932;0.028003;-0.492375;,
  -0.327339;0.806377;-0.492550;,
  0.303051;-0.818005;-0.488905;,
  -0.557127;0.653450;-0.512458;,
  0.519973;-0.689453;-0.504265;,
  -0.836461;0.246978;-0.489218;,
  0.812804;-0.310669;-0.492782;,
  -0.878428;0.002488;-0.477869;,
  0.875297;-0.029807;-0.482666;;
  15;
  4;0,0,0,0;,
  4;1,1,1,1;,
  4;2,2,2,2;,
  4;6,6,8,8;,
  4;1,1,1,1;,
  4;5,5,7,7;,
  4;8,8,10,10;,
  4;1,1,1,1;,
  4;7,7,9,9;,
  4;10,10,12,12;,
  4;1,1,1,1;,
  4;9,9,11,11;,
  4;12,12,4,4;,
  4;1,1,1,1;,
  4;11,11,3,3;;
 }
 MeshTextureCoords {
  28;
  0.000000;0.000000;,
  0.166670;0.000000;,
  0.166670;1.000000;,
  0.000000;1.000000;,
  0.333330;0.000000;,
  0.333330;1.000000;,
  0.500000;0.000000;,
  0.500000;1.000000;,
  0.777780;0.500000;,
  0.777780;0.666670;,
  0.833330;0.666670;,
  0.833330;0.500000;,
  0.777780;0.833330;,
  0.833330;0.833330;,
  0.777780;1.000000;,
  0.833330;1.000000;,
  0.888890;0.666670;,
  0.888890;0.500000;,
  0.888890;0.833330;,
  0.888890;1.000000;,
  0.944440;0.666670;,
  0.944440;0.500000;,
  0.944440;0.833330;,
  0.944440;1.000000;,
  1.000000;0.666670;,
  1.000000;0.500000;,
  1.000000;0.833330;,
  1.000000;1.000000;;
 }
}
