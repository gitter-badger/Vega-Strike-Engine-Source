#include <string>
#include <vector>
///Stores all the load-time vertex info in the XML struct FIXME light calculations
#include "expat.h"

using std::vector;
using std::string;
#include "xml_support.h"
#include "hashtable.h"
using namespace XMLSupport;
struct GFXVertex {
  float x,y,z;
  float i,j,k;
  float s,t;
  GFXVertex operator * (float s) {
    GFXVertex ret (*this);
    ret.x*=s;
    ret.y*=s;
    ret.z*=s;
	return ret;
  }
};

/**
 * holds all parameters for materials
 */
struct GFXMaterial
{
  /// ambient rgba, if you don't like these things, ask me to rename them
	float ar;float ag;float ab;float aa;
  /// diffuse rgba
	float dr;float dg;float db;float da;
  /// specular rgba
	float sr;float sg;float sb;float sa;
  /// emissive rgba
	float er;float eg;float eb;float ea;
  /// specular power
	float power; 
};


class Vector { public: float x; float y; float z; Vector(){}; Vector (float a, float b, float c)  {x=a;y=b;z=c;}};

///Stores all the load-time vertex info in the XML struct FIXME light calculations
  struct XML {
	  XML(){};
    enum Names {
      //elements
      UNKNOWN, 
      MATERIAL,
      AMBIENT,
      DIFFUSE,
      SPECULAR,
      EMISSIVE,
      MESH, 
      POINTS, 
      POINT, 
      LOCATION, 
      NORMAL, 
      POLYGONS,
      LINE,
      LOD,
      TRI, 
      QUAD,
      LODFILE,
      LINESTRIP,
      TRISTRIP,
      TRIFAN,
      QUADSTRIP,
      VERTEX,
      LOGO,
      REF,
      //attributes
      POWER,
      REFLECT,
      CULLFACE,
      LIGHTINGON,
      FLATSHADE,
      TEXTURE,
      FORCETEXTURE,
      ALPHAMAP,
      SHAREVERT,
      ALPHA,
      RED,
      GREEN,
      BLUE,
      X,
      Y,
      Z,
      I,
      J,
      K,
      S,
      T,
      SCALE,
      BLENDMODE,
      TYPE,
      ROTATE,
      WEIGHT,
      SIZE,
      OFFSET,
      ANIMATEDTEXTURE,
      USENORMALS,
      REVERSE,
	  POLYGONOFFSET,
	  DETAILTEXTURE,
	  DETAILPLANE,
	  FRAMESPERSECOND,
	  STARTFRAME
    };
    ///Saves which attributes of vertex have been set in XML file
    enum PointState {
      P_X = 0x1,
      P_Y = 0x2,
      P_Z = 0x4,
      P_I = 0x8,
      P_J = 0x10,
      P_K = 0x20
    };
    ///Saves which attributes of vertex have been set in Polygon for XML file
    enum VertexState {
      V_POINT = 0x1,
      V_S = 0x2,
      V_T = 0x4
    };
    ///Save if various logo values have been set
    enum LogoState {
      V_TYPE = 0x1,
      V_ROTATE = 0x2,
      V_SIZE=0x4,
      V_OFFSET=0x8,
      V_REF=0x10
    };
    ///To save the constructing of a logo
    struct ZeLogo {
      ///Which type the logo is (0 = faction 1 = squad >2 = internal use
      unsigned int type;
      ///how many degrees logo is rotated
      float rotate;
      ///Size of the logo
      float size;
      ///offset of polygon of logo
      float offset;
      ///the reference points that the logo is weighted against
      vector <int> refpnt;
      ///the weight of the points in weighted average of refpnts
      vector <float> refweight;
    };
    struct ZeTexture {
        string decal_name;
        string alpha_name;
        string animated_name;
    };
    class Flightgroup * fg;
    static const EnumMap::Pair element_names[];
    static const EnumMap::Pair attribute_names[];
    static const EnumMap element_map;
    static const EnumMap attribute_map;
    ///All logos on this unit
    vector <ZeLogo> logos;
    vector<Names> state_stack;
    bool sharevert;
    bool usenormals;
    bool reverse;
    bool force_texture;
    int load_stage;
    int point_state;
    int vertex_state;
	float scale;
    Vector lodscale;
    vector <ZeTexture> decals;
    bool recalc_norm;
    int num_vertices;
    vector<GFXVertex> vertices;
    ///keep count to make averaging easy 
    vector<int>vertexcount;
    vector<GFXVertex> lines;
    vector<GFXVertex> tris;
    vector<GFXVertex> quads;
    vector <vector<GFXVertex> > linestrips;
    vector <vector<GFXVertex> > tristrips;
    vector <vector<GFXVertex> > trifans;
    vector <vector<GFXVertex> > quadstrips;
    int tstrcnt;
    int tfancnt;
    int qstrcnt;
    int lstrcnt;
    vector<int> lineind;
    vector<int> nrmllinstrip;
    vector<int> linestripind;
    ///for possible normal computation
    vector<int> triind;
    vector<int> nrmltristrip;
    vector<int> tristripind;
    vector<int> nrmltrifan;
    vector<int> trifanind;
    vector<int> nrmlquadstrip;
    vector<int> quadstripind;
    vector<int> quadind;
    vector<int> trishade;
    vector<int> quadshade;
    vector<int> *active_shade;
    vector<GFXVertex> *active_list;
    vector<int> *active_ind;
    vector <string> lod;
    vector <float> lodsize;
    GFXVertex vertex;
    GFXMaterial material;
    int faction;
  };

using XMLSupport::EnumMap;
using XMLSupport::Attribute;
using XMLSupport::AttributeList;
using XMLSupport::parse_float;
using XMLSupport::parse_bool;
using XMLSupport::parse_int;

const EnumMap::Pair XML::element_names[] = {
  EnumMap::Pair("UNKNOWN", XML::UNKNOWN),
  EnumMap::Pair("Material", XML::MATERIAL),
  EnumMap::Pair("LOD", XML::LOD),
  EnumMap::Pair("Ambient", XML::AMBIENT),
  EnumMap::Pair("Diffuse", XML::DIFFUSE),
  EnumMap::Pair("Specular", XML::SPECULAR),
  EnumMap::Pair("Emissive", XML::EMISSIVE),
  EnumMap::Pair("Mesh", XML::MESH),
  EnumMap::Pair("Points", XML::POINTS),
  EnumMap::Pair("Point", XML::POINT),
  EnumMap::Pair("Location", XML::LOCATION),
  EnumMap::Pair("Normal", XML::NORMAL),
  EnumMap::Pair("Polygons", XML::POLYGONS),
  EnumMap::Pair("Line", XML::LINE),
  EnumMap::Pair("Tri", XML::TRI),
  EnumMap::Pair("Quad", XML::QUAD),
  EnumMap::Pair("Linestrip",XML::LINESTRIP),
  EnumMap::Pair("Tristrip", XML::TRISTRIP),
  EnumMap::Pair("Trifan", XML::TRIFAN),
  EnumMap::Pair("Quadstrip", XML::QUADSTRIP),
  EnumMap::Pair("Vertex", XML::VERTEX),
  EnumMap::Pair("Logo", XML::LOGO),
  EnumMap::Pair("Ref",XML::REF),
  EnumMap::Pair("DetailPlane",XML::DETAILPLANE)
};

const EnumMap::Pair XML::attribute_names[] = {
  EnumMap::Pair("UNKNOWN", XML::UNKNOWN),
  EnumMap::Pair("Scale",XML::SCALE),
  EnumMap::Pair("Blend",XML::BLENDMODE),
  EnumMap::Pair("texture", XML::TEXTURE),
  EnumMap::Pair("alphamap", XML::ALPHAMAP),
  EnumMap::Pair("sharevertex", XML::SHAREVERT),
  EnumMap::Pair("red", XML::RED),
  EnumMap::Pair("green", XML::GREEN),
  EnumMap::Pair("blue", XML::BLUE),
  EnumMap::Pair("alpha", XML::ALPHA),
  EnumMap::Pair("power", XML::POWER),
  EnumMap::Pair("reflect", XML::REFLECT),
  EnumMap::Pair("x", XML::X),
  EnumMap::Pair("y", XML::Y),
  EnumMap::Pair("z", XML::Z),
  EnumMap::Pair("i", XML::I),
  EnumMap::Pair("j", XML::J),
  EnumMap::Pair("k", XML::K),
  EnumMap::Pair("Shade", XML::FLATSHADE),
  EnumMap::Pair("point", XML::POINT),
  EnumMap::Pair("s", XML::S),
  EnumMap::Pair("t", XML::T),
  //Logo stuffs
  EnumMap::Pair("Type",XML::TYPE),
  EnumMap::Pair("Rotate", XML::ROTATE),
  EnumMap::Pair("Weight", XML::WEIGHT),
  EnumMap::Pair("Size", XML::SIZE),
  EnumMap::Pair("Offset",XML::OFFSET),
  EnumMap::Pair("meshfile",XML::LODFILE),
  EnumMap::Pair ("Animation",XML::ANIMATEDTEXTURE),
  EnumMap::Pair ("Reverse",XML::REVERSE),
  EnumMap::Pair ("LightingOn",XML::LIGHTINGON),
  EnumMap::Pair ("CullFace",XML::CULLFACE),
  EnumMap::Pair ("ForceTexture",XML::FORCETEXTURE),
  EnumMap::Pair ("UseNormals",XML::USENORMALS),
  EnumMap::Pair ("PolygonOffset",XML::POLYGONOFFSET),
  EnumMap::Pair ("DetailTexture",XML::DETAILTEXTURE),
  EnumMap::Pair ("FramesPerSecond",XML::FRAMESPERSECOND)
};



const EnumMap XML::element_map(XML::element_names, 24);
const EnumMap XML::attribute_map(XML::attribute_names, 37);

enum BLENDFUNC{
    ZERO            = 1,
	ONE             = 2, 
    SRCCOLOR        = 3,
	INVSRCCOLOR     = 4, 
    SRCALPHA        = 5,
	INVSRCALPHA     = 6, 
    DESTALPHA       = 7,
	INVDESTALPHA    = 8, 
    DESTCOLOR       = 9,
	INVDESTCOLOR    = 10, 
    SRCALPHASAT     = 11,
    CONSTALPHA    = 12, 
    INVCONSTALPHA = 13,
    CONSTCOLOR = 14,
    INVCONSTCOLOR = 15
};


enum BLENDFUNC parse_alpha (const char * tmp ) {
  if (strcmp (tmp,"ZERO")==0) {
    return ZERO;
  }

  if (strcmp (tmp,"ONE")==0) {
    return ONE;
  }

  if (strcmp (tmp,"SRCCOLOR")==0) {
    return SRCCOLOR;
  }

  if (strcmp (tmp,"INVSRCCOLOR")==0) {
    return INVSRCCOLOR;
  }

  if (strcmp (tmp,"SRCALPHA")==0) {
    return SRCALPHA;
  }

  if (strcmp (tmp,"INVSRCALPHA")==0) {
    return INVSRCALPHA;
  }

  if (strcmp (tmp,"DESTALPHA")==0) {
    return DESTALPHA;
  }

  if (strcmp (tmp,"INVDESTALPHA")==0) {
    return INVDESTALPHA;
  }

  if (strcmp (tmp,"DESTCOLOR")==0) {
    return DESTCOLOR;
  }

  if (strcmp (tmp,"INVDESTCOLOR")==0) {
    return INVDESTCOLOR;
  }

  if (strcmp (tmp,"SRCALPHASAT")==0) {
    return SRCALPHASAT;
  }

  if (strcmp (tmp,"CONSTALPHA")==0) {
    return CONSTALPHA;
  }

  if (strcmp (tmp,"INVCONSTALPHA")==0) {
    return INVCONSTALPHA;
  }

  if (strcmp (tmp,"CONSTCOLOR")==0) {
    return CONSTCOLOR;
  }

  if (strcmp (tmp,"INVCONSTCOLOR")==0) {
    return INVCONSTCOLOR;
  }

  return ZERO;
}





/* Load stages:

0 - no tags seen
1 - waiting for points
2 - processing points 
3 - done processing points, waiting for face data

Note that this is only a debugging aid. Once DTD is written, there
will be no need for this sort of checking
 */


bool shouldreflect (string r) {
    if (strtoupper(r)=="FALSE")
		return false;
	int i;
	for (i=0;i<(int)r.length();++i) {
		if (r[i]!='0'&&r[i]!='.'&&r[i]!='+'&&r[i]!='e')
			return true;
	}
	return false;
}




void beginElement(const string &name, const AttributeList &attributes, XML * xml) {
  
  AttributeList::const_iterator iter;
  XML::Names elem = (XML::Names)XML::element_map.lookup(name);
  XML::Names top;
  if(xml->state_stack.size()>0) top = *xml->state_stack.rbegin();
  xml->state_stack.push_back(elem);
  switch(elem) {
	  case XML::MATERIAL:
  case XML::DIFFUSE:
	  break;
  case XML::EMISSIVE:
	  break;
  case XML::SPECULAR:
	  break;
  case XML::AMBIENT:
	  break;
  case XML::UNKNOWN:
   fprintf (stderr, "Unknown element start tag '%s' detected\n",name.c_str());
    break;
  case XML::MESH:
    for(iter = attributes.begin(); iter!=attributes.end(); iter++) {
      switch(XML::attribute_map.lookup((*iter).name)) {
      case XML::ANIMATEDTEXTURE:
	break;
      case XML::REVERSE:
	break;
      case XML::FORCETEXTURE:
	break;
      case XML::TEXTURE:
	break;
      case XML::ALPHAMAP:
	break;
      case XML::SCALE:
	break;
      case XML::SHAREVERT:
	break;
      case XML::BLENDMODE:
	break;
      }
    }break;
  case XML::POINTS:
    break;
  case XML::POINT:
 //   assert(top==XML::POINTS);
    break;
  case XML::LOCATION:
    for(iter = attributes.begin(); iter!=attributes.end(); iter++) {
      switch(XML::attribute_map.lookup((*iter).name)) {
      case XML::X:
	xml->vertex.x = XMLSupport::parse_float((*iter).value);
	break;
      case XML::Y:
	xml->vertex.y = XMLSupport::parse_float((*iter).value);
	break;
     case XML::Z:
	xml->vertex.z = XMLSupport::parse_float((*iter).value);
	break;
      case XML::S:
	xml->vertex.s = XMLSupport::parse_float ((*iter).value);
	break;
      case XML::T:
	xml->vertex.t = XMLSupport::parse_float ((*iter).value);
	break;
      }
    }
    break;
  case XML::NORMAL:
    for(iter = attributes.begin(); iter!=attributes.end(); iter++) {
      switch(XML::attribute_map.lookup((*iter).name)) {
      case XML::UNKNOWN:
	fprintf (stderr, "Unknown attribute '%s' encountered in Normal tag\n",(*iter).name.c_str());
	break;
      case XML::I:
	xml->vertex.i = XMLSupport::parse_float((*iter).value);
	break;
      case XML::J:
	xml->vertex.j = XMLSupport::parse_float((*iter).value);
	break;
      case XML::K:
	xml->vertex.k = XMLSupport::parse_float((*iter).value);
	break;
      }
    }
    break;
  case XML::POLYGONS:
    break;
  case XML::LINE:
    break;
  case XML::TRI:
    break;
  case XML::LINESTRIP:
    break;

  case XML::TRISTRIP:
    break;

  case XML::TRIFAN:
    break;

  case XML::QUADSTRIP:
    break;
   
  case XML::QUAD:
    break;
  case XML::LOD: 
    break;
  case XML::VERTEX:
    break;
  case XML::LOGO: 
    break;
  case XML::REF:
    break;
      }
    
}

void endElement(const string &name, XML * xml) {
  xml->state_stack.pop_back();
  XML::Names elem = (XML::Names)XML::element_map.lookup(name);

  unsigned int i;
  switch(elem) {
  case XML::UNKNOWN:
    fprintf (stderr,"Unknown element end tag '%s' detected\n",name.c_str());
    break;
  case XML::POINT:
    xml->vertices.push_back (xml->vertex/**xml->scale*/);
    break;
  case XML::POINTS:
    break;
  case XML::LINE:
    break;
  case XML::TRI:
    break;
  case XML::QUAD:
    break;
  case XML::LINESTRIP:
    break;
  case XML::TRISTRIP:
    break;
  case XML::TRIFAN:
    break;
  case XML::QUADSTRIP://have to fix up nrmlquadstrip so that it 'looks' like a quad list for smooth shading
    break;
  case XML::POLYGONS:
    break;
  case XML::REF:
    break;
  case XML::LOGO:
    break;
  case XML::MATERIAL:
	  break;
  case XML::DIFFUSE:
	  break;
  case XML::EMISSIVE:
	  break;
  case XML::SPECULAR:
	  break;
  case XML::AMBIENT:
	  break;
  case XML::MESH:
    break;
  default:
    ;
  }
}

void beginElement(void *userData, const XML_Char *name, const XML_Char **atts) {
  beginElement(name, AttributeList(atts),(XML *)userData);
}



void endElement(void *userData, const XML_Char *name) {
  endElement(name, (XML *) userData);
}


XML LoadXML(const char *filename, float unitscale) {
  const int chunk_size = 1048576;
  FILE* inFile = fopen (filename, "r");
  if(!inFile) {
    fprintf (stderr,"Cannot Open Mesh File %s\n",filename);
    exit(-1);
  }

  XML xml;
  xml.scale = unitscale;
  XML_Parser parser = XML_ParserCreate(NULL);
  XML_SetUserData(parser, &xml);
  XML_SetElementHandler(parser, &beginElement, &endElement);
  
  do {
    char buf[chunk_size];
    int length;
    
    length = fread(buf,1, chunk_size,inFile);
    XML_Parse (parser,buf,length,feof(inFile));
  } while(!feof(inFile));
  fclose (inFile);
  XML_ParserFree (parser);
  // Now, copy everything into the mesh data structures
  return xml;
}


int main (int argc, char** argv) {
	if (argc!=3){
		fprintf(stderr,"wrong number of arguments, aborting\n");
		for(int i = 0; i<argc;i++){
			fprintf(stderr,"%d : %s\n",i,argv[i]);
		}
		exit(-1);
	}

  XML stub =(LoadXML(argv[1],1));
  FILE * Outputfile=fopen(argv[2],"w"); 
  float versionnumber=(1.0)/(16.0);
  int numvertsstub= stub.vertices.size();
  fwrite(&versionnumber,4,1,Outputfile);
  fwrite(&numvertsstub,4,1,Outputfile);
  return 0;
}








/*
void Mesh::beginElement(const string &name, const AttributeList &attributes) {
	static bool use_detail_texture = XMLSupport::parse_bool(vs_config->getVariable("graphics","use_detail_texture","true"));
  //static bool flatshadeit=false;
  AttributeList::const_iterator iter;
  float flotsize=1;
  XML::Names elem = (XML::Names)XML::element_map.lookup(name);
  XML::Names top;
  if(xml->state_stack.size()>0) top = *xml->state_stack.rbegin();
  xml->state_stack.push_back(elem);
  bool texture_found = false;
  switch(elem) {
      case XML::DETAILPLANE:
	  if (use_detail_texture) {
		  Vector vec (detailPlanes.size()>=2?1:0,detailPlanes.size()==1?1:0,detailPlanes.size()==0?1:0);
		  for(iter = attributes.begin(); iter!=attributes.end(); iter++) {
		    switch(XML::attribute_map.lookup((*iter).name)) {
			case XML::X:
				vec.i=XMLSupport::parse_float(iter->value);
				break;
			case XML::Y:
				vec.j=XMLSupport::parse_float(iter->value);
				break;
				
			case XML::Z:
				vec.k=XMLSupport::parse_float(iter->value);
				break;
			}
		  }
		  static float detailscale = XMLSupport::parse_float(vs_config->getVariable("graphics","detail_texture_scale","1"));
		  if (detailPlanes.size()<6) {
			  detailPlanes.push_back(vec*detailscale);
		  }

	  }
	  break;
	  case XML::MATERIAL:
	    //		  assert(xml->load_stage==4);
		  xml->load_stage=7;
		  for(iter = attributes.begin(); iter!=attributes.end(); iter++) {
		    switch(XML::attribute_map.lookup((*iter).name)) {
			case XML::USENORMALS:
			  xml->usenormals = XMLSupport::parse_bool (iter->value);
			  break;
		    case XML::POWER:
		      xml->material.power=XMLSupport::parse_float((*iter).value);
		      break;
		    case XML::REFLECT:
		      setEnvMap ( shouldreflect((*iter).value));
		      break;
		    case XML::LIGHTINGON:
		      setLighting (XMLSupport::parse_bool (vs_config->getVariable ("graphics","ForceLighting","true"))||XMLSupport::parse_bool((*iter).value)); 
		      break;
		    case XML::CULLFACE:
		      forceCullFace (XMLSupport::parse_bool((*iter).value)); 
		      break;
		    }
		  }
		  break;
  case XML::DIFFUSE:
	  //assert(xml->load_stage==7);
	  xml->load_stage=8;
	  for(iter = attributes.begin(); iter!=attributes.end(); iter++) {
		  switch(XML::attribute_map.lookup((*iter).name)) {
		  case XML::RED:
			  xml->material.dr=XMLSupport::parse_float((*iter).value);
			  break;
		  case XML::BLUE:
			  xml->material.db=XMLSupport::parse_float((*iter).value);
			  break;
		  case XML::ALPHA:
			  xml->material.da=XMLSupport::parse_float((*iter).value);
			  break;
		  case XML::GREEN:
			  xml->material.dg=XMLSupport::parse_float((*iter).value);
			  break;
		  }
	  }
	  break;
  case XML::EMISSIVE:
	  //assert(xml->load_stage==7);
	  xml->load_stage=8;
	  for(iter = attributes.begin(); iter!=attributes.end(); iter++) {
		  switch(XML::attribute_map.lookup((*iter).name)) {
		  case XML::RED:
			  xml->material.er=XMLSupport::parse_float((*iter).value);
			  break;
		  case XML::BLUE:
			  xml->material.eb=XMLSupport::parse_float((*iter).value);
			  break;
		  case XML::ALPHA:
			  xml->material.ea=XMLSupport::parse_float((*iter).value);
			  break;
		  case XML::GREEN:
			  xml->material.eg=XMLSupport::parse_float((*iter).value);
			  break;
		  }
	  }
	  break;
  case XML::SPECULAR:
	  //assert(xml->load_stage==7);
	  xml->load_stage=8;
	  for(iter = attributes.begin(); iter!=attributes.end(); iter++) {
		  switch(XML::attribute_map.lookup((*iter).name)) {
		  case XML::RED:
			  xml->material.sr=XMLSupport::parse_float((*iter).value);
			  break;
		  case XML::BLUE:
			  xml->material.sb=XMLSupport::parse_float((*iter).value);
			  break;
		  case XML::ALPHA:
			  xml->material.sa=XMLSupport::parse_float((*iter).value);
			  break;
		  case XML::GREEN:
			  xml->material.sg=XMLSupport::parse_float((*iter).value);
			  break;
		  }
	  }
	  break;
  case XML::AMBIENT:
	  //assert(xml->load_stage==7);
	  xml->load_stage=8;
	  for(iter = attributes.begin(); iter!=attributes.end(); iter++) {
		  switch(XML::attribute_map.lookup((*iter).name)) {
		  case XML::RED:
			  xml->material.ar=XMLSupport::parse_float((*iter).value);
			  break;
		  case XML::BLUE:
			  xml->material.ab=XMLSupport::parse_float((*iter).value);
			  break;
		  case XML::ALPHA:
			  xml->material.aa=XMLSupport::parse_float((*iter).value);
			  break;
		  case XML::GREEN:
			  xml->material.ag=XMLSupport::parse_float((*iter).value);
			  break;
		  }
	  }
	  break;
  case XML::UNKNOWN:
   VSFileSystem::vs_fprintf (stderr, "Unknown element start tag '%s' detected\n",name.c_str());
    break;
  case XML::MESH:
    assert(xml->load_stage == 0);
    assert(xml->state_stack.size()==1);
    xml->load_stage = 1;
    // Read in texture attribute
    for(iter = attributes.begin(); iter!=attributes.end(); iter++) {
      switch(XML::attribute_map.lookup((*iter).name)) {
      case XML::REVERSE:
	xml->reverse = XMLSupport::parse_bool((*iter).value);
	break;
      case XML::FORCETEXTURE:
	xml->force_texture=XMLSupport::parse_bool ((*iter).value);
	break;
      case XML::SCALE:
	xml->scale *=  XMLSupport::parse_float ((*iter).value);
	break;
      case XML::SHAREVERT:
	xml->sharevert = (XMLSupport::parse_bool ((*iter).value)&&XMLSupport::parse_bool (vs_config->getVariable ("graphics","SharedVertexArrays","true")));
	break;
	  case XML::POLYGONOFFSET:
		  this->polygon_offset = XMLSupport::parse_float ((*iter).value);
		  break;
      case XML::BLENDMODE:
	{
	  char *csrc=strdup ((*iter).value.c_str());
	  char *cdst=strdup((*iter).value.c_str());
	  sscanf (((*iter).value).c_str(),"%s %s",csrc,cdst);
	  SetBlendMode (parse_alpha (csrc),parse_alpha (cdst));
	  free (csrc);
	  free (cdst);
	}
	break;
	  case XML::DETAILTEXTURE:
		  if (use_detail_texture)
			  detailTexture = TempGetTexture(iter->value,FactionUtil::GetFaction(xml->faction),GFXTRUE);
		  break;
      case XML::TEXTURE:
          //NO BREAK..goes to next statement
      case XML::ALPHAMAP:
      case XML::ANIMATEDTEXTURE:
      case XML::UNKNOWN:
        {
          XML::Names whichtype = XML::UNKNOWN;
          int strsize=0;
          if (strtoupper(iter->name).find("ANIMATION")==0) {
              whichtype = XML::ANIMATEDTEXTURE;
              strsize = strlen ("ANIMATION");
          }
          if (strtoupper(iter->name).find("TEXTURE")==0){
              whichtype= XML::TEXTURE;
              strsize = strlen ("TEXTURE");
          }
          if (strtoupper(iter->name).find("ALPHAMAP")==0){
              whichtype=XML::ALPHAMAP;
              strsize= strlen ("ALPHAMAP");
          }
          if (whichtype!=XML::UNKNOWN) {
              unsigned int texindex =0;
              string ind(iter->name.substr (strsize));
              if (!ind.empty())
                  texindex=XMLSupport::parse_int(ind);
			  static bool per_pixel_lighting = XMLSupport::parse_bool (vs_config->getVariable ("graphics","per_pixel_lighting","true"));
			  if (texindex==0||per_pixel_lighting) {
              while (xml->decals.size()<=texindex)
                  xml->decals.push_back(XML::ZeTexture());
              switch (whichtype) {
                  case XML::ANIMATEDTEXTURE:
                      xml->decals[texindex].animated_name=iter->value;
                      break;
                  case XML::ALPHAMAP:
                      xml->decals[texindex].alpha_name=iter->value;
                      break;
                  default:
                      xml->decals[texindex].decal_name=iter->value;
              }
			  }
              if (texindex==0) {
                  texture_found = true;                  
              }
          }
        }
        break;
      }
    }
    assert(texture_found);
    break;
  case XML::POINTS:
    assert(top==XML::MESH);
    //assert(xml->load_stage == 1);
    xml->load_stage = 2;
    break;
  case XML::POINT:
    assert(top==XML::POINTS);
    
    memset(&xml->vertex, 0, sizeof(xml->vertex));
    xml->point_state = 0; // Point state is used to check that all necessary attributes are recorded
    break;
  case XML::LOCATION:
    assert(top==XML::POINT);
    for(iter = attributes.begin(); iter!=attributes.end(); iter++) {
      switch(XML::attribute_map.lookup((*iter).name)) {
      case XML::UNKNOWN:
	VSFileSystem::vs_fprintf (stderr, "Unknown attribute '%s' encountered in Location tag\n",(*iter).name.c_str());
	break;
      case XML::X:
	assert(!(xml->point_state & XML::P_X));
	xml->vertex.x = XMLSupport::parse_float((*iter).value);
	xml->vertex.i = 0;
	xml->point_state |= XML::P_X;
	break;
      case XML::Y:
	assert(!(xml->point_state & XML::P_Y));
	xml->vertex.y = XMLSupport::parse_float((*iter).value);
	xml->vertex.j = 0;
	xml->point_state |= XML::P_Y;
	break;
     case XML::Z:
	assert(!(xml->point_state & XML::P_Z));
	xml->vertex.z = XMLSupport::parse_float((*iter).value);
	xml->vertex.k = 0;
	xml->point_state |= XML::P_Z;
	break;
      case XML::S:
	xml->vertex.s = XMLSupport::parse_float ((*iter).value);
	break;
      case XML::T:
	xml->vertex.t = XMLSupport::parse_float ((*iter).value);
	break;
      default:
	assert(0);
      }
    }
    assert(xml->point_state & (XML::P_X |
			       XML::P_Y |
			       XML::P_Z) == 
	   (XML::P_X |
	    XML::P_Y |
	    XML::P_Z) );
    break;
  case XML::NORMAL:
    assert(top==XML::POINT);
    for(iter = attributes.begin(); iter!=attributes.end(); iter++) {
      switch(XML::attribute_map.lookup((*iter).name)) {
      case XML::UNKNOWN:
	VSFileSystem::vs_fprintf (stderr, "Unknown attribute '%s' encountered in Normal tag\n",(*iter).name.c_str());
	break;
      case XML::I:
	assert(!(xml->point_state & XML::P_I));
	xml->vertex.i = XMLSupport::parse_float((*iter).value);
	xml->point_state |= XML::P_I;
	break;
      case XML::J:
	assert(!(xml->point_state & XML::P_J));
	xml->vertex.j = XMLSupport::parse_float((*iter).value);
	xml->point_state |= XML::P_J;
	break;
      case XML::K:
	assert(!(xml->point_state & XML::P_K));
	xml->vertex.k = XMLSupport::parse_float((*iter).value);
	xml->point_state |= XML::P_K;
	break;
      default:
	assert(0);
      }
    }
    if (xml->point_state & (XML::P_I |
			       XML::P_J |
			       XML::P_K) != 
	   (XML::P_I |
	    XML::P_J |
	    XML::P_K) ) {
      if (!xml->recalc_norm) {
	xml->vertex.i=xml->vertex.j=xml->vertex.k=0;
	xml->recalc_norm=true;
      }
    }
    break;
  case XML::POLYGONS:
    assert(top==XML::MESH);
    //assert(xml->load_stage==3);
    xml->load_stage = 4;
    break;
  case XML::LINE:
    assert(top==XML::POLYGONS);
    //assert(xml->load_stage==4);
    xml->num_vertices=2;
    xml->active_list = &xml->lines;
    xml->active_ind = &xml->lineind;
    for(iter = attributes.begin(); iter!=attributes.end(); iter++) {
      switch(XML::attribute_map.lookup((*iter).name)) {
      case XML::UNKNOWN:
	VSFileSystem::vs_fprintf (stderr,"Unknown attribute '%s' encountered in Vertex tag\n",(*iter).name.c_str() );
	break;
      case XML::FLATSHADE:
	if ((*iter).value=="Flat") {
	  VSFileSystem::vs_fprintf (stderr,"Cannot Flatshade Lines\n");
	}else {
	  if ((*iter).value=="Smooth") {
	    //ignored -- already done
	  }
	}
	break;
      default:
	assert (0);
      }
    }
    break;
  case XML::TRI:
    assert(top==XML::POLYGONS);
    //assert(xml->load_stage==4);
    xml->num_vertices=3;
    xml->active_list = &xml->tris;
    xml->active_ind = &xml->triind;
    xml->trishade.push_back (0);
    for(iter = attributes.begin(); iter!=attributes.end(); iter++) {
      switch(XML::attribute_map.lookup((*iter).name)) {
      case XML::UNKNOWN:
	VSFileSystem::vs_fprintf (stderr,"Unknown attribute '%s' encountered in Vertex tag\n",(*iter).name.c_str() );
	break;
      case XML::FLATSHADE:
	if ((*iter).value=="Flat") {
	  xml->trishade[xml->trishade.size()-1]=1;
	}else {
	  if ((*iter).value=="Smooth") {
	    xml->trishade[xml->trishade.size()-1]=0;
	  }
	}
	break;
      default:
	assert (0);
      }
    }
    break;
  case XML::LINESTRIP:
    assert(top==XML::POLYGONS);
    //assert(xml->load_stage==4);
    xml->num_vertices=2;
    xml->linestrips.push_back (vector<GFXVertex>());
    xml->active_list = &(xml->linestrips[xml->linestrips.size()-1]);
    xml->lstrcnt = xml->linestripind.size();
    xml->active_ind = &xml->linestripind;
    for(iter = attributes.begin(); iter!=attributes.end(); iter++) {
      switch(XML::attribute_map.lookup((*iter).name)) {
      case XML::UNKNOWN:
	VSFileSystem::vs_fprintf (stderr,"Unknown attribute '%s' encountered in Vertex tag\n",(*iter).name.c_str() );
	break;
      case XML::FLATSHADE:
	if ((*iter).value=="Flat") {
	  VSFileSystem::vs_fprintf(stderr,"Cannot Flatshade Linestrips\n");
	}else {
	  if ((*iter).value=="Smooth") {
	    //ignored -- already done
	  }
	}
	break;
      default:
	assert (0);
      }
    }
    break;

  case XML::TRISTRIP:
    assert(top==XML::POLYGONS);
    //assert(xml->load_stage==4);
    xml->num_vertices=3;//minimum number vertices
    xml->tristrips.push_back (vector<GFXVertex>());
    xml->active_list = &(xml->tristrips[xml->tristrips.size()-1]);
    xml->tstrcnt = xml->tristripind.size();
    xml->active_ind = &xml->tristripind;
    for(iter = attributes.begin(); iter!=attributes.end(); iter++) {
      switch(XML::attribute_map.lookup((*iter).name)) {
      case XML::UNKNOWN:
	VSFileSystem::vs_fprintf (stderr,"Unknown attribute '%s' encountered in Vertex tag\n",(*iter).name.c_str() );
	break;
      case XML::FLATSHADE:
	if ((*iter).value=="Flat") {
	  VSFileSystem::vs_fprintf(stderr,"Cannot Flatshade Tristrips\n");
	}else {
	  if ((*iter).value=="Smooth") {
	    //ignored -- already done
	  }
	}
	break;
      default:
	assert (0);
      }
    }
    break;

  case XML::TRIFAN:
    assert(top==XML::POLYGONS);
    //assert(xml->load_stage==4);
    xml->num_vertices=3;//minimum number vertices
    xml->trifans.push_back (vector<GFXVertex>());
    xml->active_list = &(xml->trifans[xml->trifans.size()-1]);
    xml->tfancnt = xml->trifanind.size();
    xml->active_ind = &xml->trifanind;
    for(iter = attributes.begin(); iter!=attributes.end(); iter++) {
      switch(XML::attribute_map.lookup((*iter).name)) {
      case XML::UNKNOWN:
	VSFileSystem::vs_fprintf (stderr,"Unknown attribute '%s' encountered in Vertex tag\n",(*iter).name.c_str() );
	break;
      case XML::FLATSHADE:
	if ((*iter).value=="Flat") {
	  VSFileSystem::vs_fprintf (stderr,"Cannot Flatshade Trifans\n");
	}else {
	  if ((*iter).value=="Smooth") {
	    //ignored -- already done
	  }
	}
	break;
      default:
	assert (0);
      }
    }
    break;

  case XML::QUADSTRIP:
    assert(top==XML::POLYGONS);
    //assert(xml->load_stage==4);
    xml->num_vertices=4;//minimum number vertices
    xml->quadstrips.push_back (vector<GFXVertex>());
    xml->active_list = &(xml->quadstrips[xml->quadstrips.size()-1]);
    xml->qstrcnt = xml->quadstripind.size();
    xml->active_ind = &xml->quadstripind;
    for(iter = attributes.begin(); iter!=attributes.end(); iter++) {
      switch(XML::attribute_map.lookup((*iter).name)) {
      case XML::UNKNOWN:
	VSFileSystem::vs_fprintf (stderr,"Unknown attribute '%s' encountered in Vertex tag\n",(*iter).name.c_str() );
	break;
      case XML::FLATSHADE:
	if ((*iter).value=="Flat") {
	  VSFileSystem::vs_fprintf (stderr, "Cannot Flatshade Quadstrips\n");
	}else {
	  if ((*iter).value=="Smooth") {
	    //ignored -- already done
	  }
	}
	break;
      default:
	assert (0);
      }
    }
    break;
   
  case XML::QUAD:
    assert(top==XML::POLYGONS);
    //assert(xml->load_stage==4);
    xml->num_vertices=4;
    xml->active_list = &xml->quads;
    xml->active_ind = &xml->quadind;
    xml->quadshade.push_back (0);
    for(iter = attributes.begin(); iter!=attributes.end(); iter++) {
      switch(XML::attribute_map.lookup((*iter).name)) {
      case XML::UNKNOWN:
	VSFileSystem::vs_fprintf (stderr,"Unknown attribute '%s' encountered in Vertex tag\n",(*iter).name.c_str() );
	break;
      case XML::FLATSHADE:
	if ((*iter).value=="Flat") {
	  xml->quadshade[xml->quadshade.size()-1]=1;
*/
/*
	}else {
	  if ((*iter).value=="Smooth") {
	    xml->quadshade[xml->quadshade.size()-1]=0;
	  }
	}
	break;
      default:
	assert(0);
      }
    }
    break;
  case XML::LOD: 
    for(iter = attributes.begin(); iter!=attributes.end(); iter++) {
      switch(XML::attribute_map.lookup((*iter).name)) {
      case XML::UNKNOWN:
	break;
	  case XML::FRAMESPERSECOND:
		  framespersecond=parse_float((*iter).value);
		  break;
      case XML::LODFILE:
	xml->lod.push_back(new Mesh ((*iter).value.c_str(),xml->lodscale,xml->faction,xml->fg,true));//make orig mesh
	break;
      case XML::SIZE:
	flotsize = XMLSupport::parse_float ((*iter).value);
	break;
      }
    }
    if (xml->lodsize.size()!=xml->lod.size()) {
      xml->lodsize.push_back (flotsize);
    }
    break;
  case XML::VERTEX:
    assert(top==XML::TRI || top==XML::QUAD || top==XML::LINE ||top ==XML::TRISTRIP || top ==XML::TRIFAN||top ==XML::QUADSTRIP || top==XML::LINESTRIP);
    //assert(xml->load_stage==4);

    xml->vertex_state = 0;
    unsigned int index;
    float s, t;
    for(iter = attributes.begin(); iter!=attributes.end(); iter++) {
      switch(XML::attribute_map.lookup((*iter).name)) {
      case XML::UNKNOWN:
	VSFileSystem::vs_fprintf (stderr,"Unknown attribute '%s' encountered in Vertex tag\n",(*iter).name.c_str() );
	break;
      case XML::POINT:
	assert(!(xml->vertex_state & XML::V_POINT));
	xml->vertex_state |= XML::V_POINT;
	index = XMLSupport::parse_int((*iter).value);
	break;
      case XML::S:
	assert(!(xml->vertex_state & XML::V_S));
	xml->vertex_state |= XML::V_S;
	s = XMLSupport::parse_float((*iter).value);
	break;
      case XML::T:
	assert(!(xml->vertex_state & XML::V_T));
	xml->vertex_state |= XML::V_T;
	t = XMLSupport::parse_float((*iter).value);
	break;
      default:
	assert(0);
     }
    }
    assert(xml->vertex_state & (XML::V_POINT|
				XML::V_S|
				XML::V_T) == 
	   (XML::V_POINT|
	    XML::V_S|
	    XML::V_T) );
    assert(index < xml->vertices.size());

    memset(&xml->vertex, 0, sizeof(xml->vertex));
    xml->vertex = xml->vertices[index];
    xml->vertexcount[index]+=1;
    if ((!xml->vertex.i)&&(!xml->vertex.j)&&(!xml->vertex.k)) {
      if (!xml->recalc_norm) {
	
	xml->recalc_norm=true;
      }
    }
    //        xml->vertex.x*=scale;
    //        xml->vertex.y*=scale;
    //        xml->vertex.z*=scale;//FIXME
    xml->vertex.s = s;
    xml->vertex.t = t;
    xml->active_list->push_back(xml->vertex);
    xml->active_ind->push_back(index);
    if (xml->reverse) {
	  unsigned int i;
      for (i=xml->active_ind->size()-1;i>0;i--) {
	(*xml->active_ind)[i]=(*xml->active_ind)[i-1];
      }
      (*xml->active_ind)[0]=index;
      for ( i=xml->active_list->size()-1;i>0;i--) {
	(*xml->active_list)[i]=(*xml->active_list)[i-1];
      }
      (*xml->active_list)[0]=xml->vertex;
    }
    xml->num_vertices--;
    break;
  case XML::LOGO: 
    assert (top==XML::MESH);
    //assert (xml->load_stage==4);
    xml->load_stage=5;
    xml->vertex_state=0;
    unsigned int typ;
    float rot, siz,offset;
    for(iter = attributes.begin(); iter!=attributes.end(); iter++) {
      switch(XML::attribute_map.lookup((*iter).name)) {
      case XML::UNKNOWN:
	VSFileSystem::vs_fprintf (stderr,"Unknown attribute '%s' encountered in Vertex tag\n",(*iter).name.c_str() );
	break;
      case XML::TYPE:
	assert (!(xml->vertex_state&XML::V_TYPE));
	xml->vertex_state|=XML::V_TYPE;
	typ = XMLSupport::parse_int((*iter).value);
	
	break;
      case XML::ROTATE:
	assert (!(xml->vertex_state&XML::V_ROTATE));
	xml->vertex_state|=XML::V_ROTATE;
	rot = XMLSupport::parse_float((*iter).value);

	break;
      case XML::SIZE:
	assert (!(xml->vertex_state&XML::V_SIZE));
	xml->vertex_state|=XML::V_SIZE;
	siz = XMLSupport::parse_float((*iter).value);
	break;
      case XML::OFFSET:
	assert (!(xml->vertex_state&XML::V_OFFSET));
	xml->vertex_state|=XML::V_OFFSET;
	offset = XMLSupport::parse_float ((*iter).value);
	break;
      default:
	assert(0);
     }
    }

    assert(xml->vertex_state & (XML::V_TYPE|
				XML::V_ROTATE|
				XML::V_SIZE|
				XML::V_OFFSET) == 
	   (XML::V_TYPE|
	    XML::V_ROTATE|
	    XML::V_SIZE|
	    XML::V_OFFSET) );
    xml->logos.push_back(XML::ZeLogo());
    xml->logos[xml->logos.size()-1].type = typ;
    xml->logos[xml->logos.size()-1].rotate = rot;
    xml->logos[xml->logos.size()-1].size = siz;
    xml->logos[xml->logos.size()-1].offset = offset;
    break;
  case XML::REF:
    {
    assert (top==XML::LOGO);
    //assert (xml->load_stage==5);
    xml->load_stage=6;
    unsigned int ind=0;
    float indweight=1;
    bool foundindex=false;
    int ttttttt;
    ttttttt=0;
    for(iter = attributes.begin(); iter!=attributes.end(); iter++) {
      switch(XML::attribute_map.lookup((*iter).name)) {
      case XML::UNKNOWN:
	VSFileSystem::vs_fprintf (stderr,"Unknown attribute '%s' encountered in Vertex tag\n",(*iter).name.c_str() );
	break;
      case XML::POINT:
	assert (ttttttt<2);
	xml->vertex_state |= XML::V_POINT;
	ind = XMLSupport::parse_int((*iter).value);
	foundindex=true;
	ttttttt+=2;
	break;
      case XML::WEIGHT:
	assert ((ttttttt&1)==0);
	ttttttt+=1;
	xml->vertex_state |= XML::V_S;
	indweight = XMLSupport::parse_float((*iter).value);
	break;
      default:
	assert(0);
     }
    }
    assert (ttttttt==3);
    if (!foundindex) {
      VSFileSystem::vs_fprintf (stderr,"mesh with uninitalized logo");
    }
    xml->logos[xml->logos.size()-1].refpnt.push_back(ind);
    xml->logos[xml->logos.size()-1].refweight.push_back(indweight);
    xml->vertex_state+=XML::V_REF;
    }
    break;
  default:
    assert(0);
  }
}
*/
/*
void Mesh::endElement(const string &name) {
  //cerr << "End tag: " << name << endl;

  XML::Names elem = (XML::Names)XML::element_map.lookup(name);
  assert(*xml->state_stack.rbegin() == elem);
  xml->state_stack.pop_back();
  unsigned int i;
  switch(elem) {
  case XML::UNKNOWN:
    VSFileSystem::vs_fprintf (stderr,"Unknown element end tag '%s' detected\n",name.c_str());
    break;
  case XML::POINT:
    assert(xml->point_state & (XML::P_X | 
			       XML::P_Y | 
			       XML::P_Z |
			       XML::P_I |
			       XML::P_J |
			       XML::P_K) == 
	   (XML::P_X | 
	    XML::P_Y | 
	    XML::P_Z |
	    XML::P_I |
	    XML::P_J |
	    XML::P_K) );
    xml->vertices.push_back(xml->vertex);
    xml->vertexcount.push_back(0);
    break;
  case XML::POINTS:
    xml->load_stage = 3;
    break;
  case XML::LINE:
    assert (xml->num_vertices==0);
    break;
  case XML::TRI:
    assert(xml->num_vertices==0);
    break;
  case XML::QUAD:
    assert(xml->num_vertices==0);
    break;
  case XML::LINESTRIP:
    assert (xml->num_vertices<=0);
    for (i=xml->lstrcnt+1;i<xml->linestripind.size();i++) {
      xml->nrmllinstrip.push_back (xml->linestripind[i-1]);
      xml->nrmllinstrip.push_back (xml->linestripind[i]);
    }
    break;
  case XML::TRISTRIP:
    assert(xml->num_vertices<=0);   
    for (i=xml->tstrcnt+2;i<xml->tristripind.size();i++) {
      if ((i-xml->tstrcnt)%2) {
	//normal order
	xml->nrmltristrip.push_back (xml->tristripind[i-2]);
	xml->nrmltristrip.push_back (xml->tristripind[i-1]);
	xml->nrmltristrip.push_back (xml->tristripind[i]);
      } else {
	//reverse order
	xml->nrmltristrip.push_back (xml->tristripind[i-1]);
	xml->nrmltristrip.push_back (xml->tristripind[i-2]);
	xml->nrmltristrip.push_back (xml->tristripind[i]);
      }
    }
    break;
  case XML::TRIFAN:
    assert (xml->num_vertices<=0);
    for (i=xml->tfancnt+2;i<xml->trifanind.size();i++) {
      xml->nrmltrifan.push_back (xml->trifanind[xml->tfancnt]);
      xml->nrmltrifan.push_back (xml->trifanind[i-1]);
      xml->nrmltrifan.push_back (xml->trifanind[i]);
    }
    break;
  case XML::QUADSTRIP://have to fix up nrmlquadstrip so that it 'looks' like a quad list for smooth shading
    assert(xml->num_vertices<=0);
    for (i=xml->qstrcnt+3;i<xml->quadstripind.size();i+=2) {
      xml->nrmlquadstrip.push_back (xml->quadstripind[i-3]);
      xml->nrmlquadstrip.push_back (xml->quadstripind[i-2]);
      xml->nrmlquadstrip.push_back (xml->quadstripind[i]);
      xml->nrmlquadstrip.push_back (xml->quadstripind[i-1]);
    }
    break;
  case XML::POLYGONS:
    assert(xml->tris.size()%3==0);
    assert(xml->quads.size()%4==0);
    break;
  case XML::REF:
    //assert (xml->load_stage==6);
    xml->load_stage=5;
    break;
  case XML::LOGO:
    //assert (xml->load_stage==5);
    assert (xml->vertex_state>=XML::V_REF*3);//make sure there are at least 3 reference points
    xml->load_stage=4;
    break;
  case XML::MATERIAL:
	  //assert(xml->load_stage==7);
	  xml->load_stage=4;
	  break;
  case XML::DETAILPLANE:
	  
	  break;
  case XML::DIFFUSE:
	  //assert(xml->load_stage==8);
	  xml->load_stage=7;
	  break;
  case XML::EMISSIVE:
	  //assert(xml->load_stage==8);
	  xml->load_stage=7;
	  break;
  case XML::SPECULAR:
	  //assert(xml->load_stage==8);
	  xml->load_stage=7;
	  break;
  case XML::AMBIENT:
	  //assert(xml->load_stage==8);
	  xml->load_stage=7;
	  break;
  case XML::MESH:
    //assert(xml->load_stage==4);//4 is done with poly, 5 is done with Logos

    xml->load_stage=5;
    break;
  default:
    ;
  }
}
*/

/*
void SumNormals (int trimax, int t3vert, 
		 vector <GFXVertex> &vertices,
		 vector <int> &triind,
		 vector <int> &vertexcount,
		 bool * vertrw ) {
  int a=0;
  int i=0;
  int j=0;
  if (t3vert==2) {//oh man we have a line--what to do, what to do!
    for (i=0;i<trimax;i++,a+=t3vert) {
      Vector Cur (vertices[triind[a]].x-vertices[triind[a+1]].x,
		  vertices[triind[a]].y-vertices[triind[a+1]].y,
		  vertices[triind[a]].z-vertices[triind[a+1]].z);
      Normalize (Cur);
      vertices[triind[a+1]].i += Cur.i/vertexcount[triind[a+1]];
      vertices[triind[a+1]].j += Cur.j/vertexcount[triind[a+1]];
      vertices[triind[a+1]].k += Cur.k/vertexcount[triind[a+1]];

      vertices[triind[a]].i -= Cur.i/vertexcount[triind[a]];
      vertices[triind[a]].j -= Cur.j/vertexcount[triind[a]];
      vertices[triind[a]].k -= Cur.k/vertexcount[triind[a]];
    }
    return; //bye bye mrs american pie 
  }
  for (i=0;i<trimax;i++,a+=t3vert) {
    for (j=0;j<t3vert;j++) {
      if (vertrw[triind[a+j]]) {
	Vector Cur (vertices[triind[a+j]].x,
		    vertices[triind[a+j]].y,
		    vertices[triind[a+j]].z);
	Cur = (Vector (vertices[triind[a+((j+2)%t3vert)]].x,
		       vertices[triind[a+((j+2)%t3vert)]].y,
		       vertices[triind[a+((j+2)%t3vert)]].z)-Cur)
	  .Cross(Vector (vertices[triind[a+((j+1)%t3vert)]].x,
			 vertices[triind[a+((j+1)%t3vert)]].y,
			 vertices[triind[a+((j+1)%t3vert)]].z)-Cur);
	const float eps = .00001;
	if (fabs(Cur.i)>eps||fabs(Cur.j)>eps||fabs(Cur.k)<eps) {
	  Normalize(Cur);	 
	  //Cur = Cur*(1.00F/xml->vertexcount[a+j]);
	  vertices[triind[a+j]].i+=Cur.i;
	  vertices[triind[a+j]].j+=Cur.j;
	  vertices[triind[a+j]].k+=Cur.k;
	}else {
	  if (vertexcount[triind[a+j]]>1) {
	    vertexcount[triind[a+j]]--;
	  }
	}
      }
    }
  }
}
*/
/*
void updateMax (Vector &mn, Vector & mx, const GFXVertex &ver) {
    mn.i = min(ver.x, mn.i);
    mx.i = max(ver.x, mx.i);
    mn.j = min(ver.y, mn.j);
    mx.j = max(ver.y, mx.j);
    mn.k = min(ver.z, mn.k);
    mx.k = max(ver.z, mx.k);
}
*/
/*
using namespace VSFileSystem;

const bool USE_RECALC_NORM=true;
const bool FLAT_SHADE=true;
*/
/*
void Mesh::LoadXML(const char *filename,const Vector& scale, int faction, Flightgroup * fg, bool origthis) {
  VSFile f;
  VSError err = f.OpenReadOnly( filename, MeshFile);
  if( err>Ok)
  {
	VSFileSystem::vs_fprintf (stderr,"Cannot Open Mesh File %s\n",filename);
	cleanexit=1;
	winsys_exit(1);
	return;
  }
  LoadXML( f, scale, faction, fg, origthis);
  f.Close();
}
*/

/*
void Mesh::LoadXML( VSFileSystem::VSFile & f, const Vector & scale, int faction, Flightgroup *fg, bool origthis)
{
  const int chunk_size = 16384;
  std::vector <unsigned int> ind;

  xml = new XML;
  xml->fg = fg;
  xml->usenormals=false;
  xml->force_texture=false;
  xml->reverse=false;
  xml->sharevert=false;
  xml->faction = faction;
  GFXGetMaterial (0, xml->material);//by default it's the default material;
  xml->load_stage = 0;
  xml->recalc_norm=false;
  xml->scale=scale;
  xml->lodscale=scale;
  XML_Parser parser = XML_ParserCreate(NULL);
  XML_SetUserData(parser, this);
  XML_SetElementHandler(parser, &Mesh::beginElement, &Mesh::endElement);

  XML_Parse (parser,(f.ReadFull()).c_str(),f.Size(),1);


  XML_ParserFree (parser);
  // Now, copy everything into the mesh data structures

  if(xml->load_stage!=5) {
    VSFileSystem::vs_fprintf (stderr,"Warning: mesh load possibly failed\n");
    exit(-1);
  }
  unsigned int i; unsigned int a=0;
  unsigned int j;
  //begin vertex normal calculations if necessary
  if (!xml->usenormals) {//fixme!


    bool *vertrw = new bool [xml->vertices.size()]; 
    for (i=0;i<xml->vertices.size();i++) {
      if (xml->vertices[i].i==0&&
	  xml->vertices[i].j==0&&
	  xml->vertices[i].k==0) {
	vertrw[i]=true;
      }else {
	vertrw[i]=USE_RECALC_NORM;
      }
    }
    SumNormals (xml->tris.size()/3,3,xml->vertices, xml->triind,xml->vertexcount, vertrw);
    SumNormals (xml->quads.size()/4,4,xml->vertices, xml->quadind,xml->vertexcount, vertrw);
    SumNormals (xml->lines.size()/2,2,xml->vertices,xml->lineind,xml->vertexcount,vertrw);
    SumNormals (xml->nrmltristrip.size()/3,3,xml->vertices, xml->nrmltristrip,xml->vertexcount, vertrw);
    SumNormals (xml->nrmltrifan.size()/3,3,xml->vertices, xml->nrmltrifan,xml->vertexcount, vertrw);
    SumNormals (xml->nrmlquadstrip.size()/4,4,xml->vertices, xml->nrmlquadstrip,xml->vertexcount, vertrw);
    SumNormals (xml->nrmllinstrip.size()/2,2,xml->vertices, xml->nrmllinstrip,xml->vertexcount, vertrw);
    delete []vertrw;
    for (i=0;i<xml->vertices.size();i++) {
      float dis = sqrtf (xml->vertices[i].i*xml->vertices[i].i +
			 xml->vertices[i].j*xml->vertices[i].j +
			 xml->vertices[i].k*xml->vertices[i].k);
      if (dis!=0) {
	xml->vertices[i].i/=dis;//renormalize
	xml->vertices[i].j/=dis;
	xml->vertices[i].k/=dis;
	  }else {
	xml->vertices[i].i=xml->vertices[i].x;
	xml->vertices[i].j=xml->vertices[i].y;
	xml->vertices[i].k=xml->vertices[i].z;
	dis = sqrtf (xml->vertices[i].i*xml->vertices[i].i +
		     xml->vertices[i].j*xml->vertices[i].j +
		     xml->vertices[i].k*xml->vertices[i].k);
	if (dis!=0) {
	  xml->vertices[i].i/=dis;//renormalize
	  xml->vertices[i].j/=dis;
	  xml->vertices[i].k/=dis;	  
	}else {
	  xml->vertices[i].i=0;
	  xml->vertices[i].j=0;
	  xml->vertices[i].k=1;	  
	}
      } 
    }
  }
    a=0;

    for (a=0;a<xml->tris.size();a+=3) {
      for (j=0;j<3;j++) {
	ind.push_back (xml->triind[a+j]);
	xml->tris[a+j].i = xml->vertices[xml->triind[a+j]].i;
	xml->tris[a+j].j = xml->vertices[xml->triind[a+j]].j;
	xml->tris[a+j].k = xml->vertices[xml->triind[a+j]].k;
      }
    }
    a=0;
    for (a=0;a<xml->quads.size();a+=4) {
      for (j=0;j<4;j++) {
	ind.push_back (xml->quadind[a+j]);
	xml->quads[a+j].i=xml->vertices[xml->quadind[a+j]].i;
	xml->quads[a+j].j=xml->vertices[xml->quadind[a+j]].j;
	xml->quads[a+j].k=xml->vertices[xml->quadind[a+j]].k;
      }
    }
    a=0;
    for (a=0;a<xml->lines.size();a+=2) {
      for (j=0;j<2;j++) {
	ind.push_back (xml->lineind[a+j]);
	xml->lines[a+j].i=xml->vertices[xml->lineind[a+j]].i;
	xml->lines[a+j].j=xml->vertices[xml->lineind[a+j]].j;
	xml->lines[a+j].k=xml->vertices[xml->lineind[a+j]].k;
      }
    }


    a=0;
    unsigned int k=0;
    unsigned int l=0;
    for (l=a=0;a<xml->tristrips.size();a++) {
      for (k=0;k<xml->tristrips[a].size();k++,l++) {
	ind.push_back (xml->tristripind[l]);
	xml->tristrips[a][k].i = xml->vertices[xml->tristripind[l]].i;
	xml->tristrips[a][k].j = xml->vertices[xml->tristripind[l]].j;
	xml->tristrips[a][k].k = xml->vertices[xml->tristripind[l]].k;
      }
    }
    for (l=a=0;a<xml->trifans.size();a++) {
      for (k=0;k<xml->trifans[a].size();k++,l++) {
	ind.push_back (xml->trifanind[l]);
	xml->trifans[a][k].i = xml->vertices[xml->trifanind[l]].i;
	xml->trifans[a][k].j = xml->vertices[xml->trifanind[l]].j;
	xml->trifans[a][k].k = xml->vertices[xml->trifanind[l]].k;
      }
    }
    for (l=a=0;a<xml->quadstrips.size();a++) {
      for (k=0;k<xml->quadstrips[a].size();k++,l++) {
	ind.push_back (xml->quadstripind[l]);
	xml->quadstrips[a][k].i = xml->vertices[xml->quadstripind[l]].i;
	xml->quadstrips[a][k].j = xml->vertices[xml->quadstripind[l]].j;
	xml->quadstrips[a][k].k = xml->vertices[xml->quadstripind[l]].k;
      }
    }
    for (l=a=0;a<xml->linestrips.size();a++) {
      for (k=0;k<xml->linestrips[a].size();k++,l++) {
	ind.push_back (xml->linestripind[l]);
	xml->linestrips[a][k].i = xml->vertices[xml->linestripind[l]].i;
	xml->linestrips[a][k].j = xml->vertices[xml->linestripind[l]].j;
	xml->linestrips[a][k].k = xml->vertices[xml->linestripind[l]].k;
      }
    }

  
  
  // TODO: add alpha handling

   //check for per-polygon flat shading
  unsigned int trimax = xml->tris.size()/3;
  a=0;
  i=0;
  j=0;
  if (!xml->usenormals) {
      for (i=0;i<trimax;i++,a+=3) {
          if (FLAT_SHADE||xml->trishade[i]==1) {
              for (j=0;j<3;j++) {
                  Vector Cur (xml->vertices[xml->triind[a+j]].x,
                              xml->vertices[xml->triind[a+j]].y,
                              xml->vertices[xml->triind[a+j]].z);
                  Cur = (Vector (xml->vertices[xml->triind[a+((j+2)%3)]].x,
                                 xml->vertices[xml->triind[a+((j+2)%3)]].y,
                                 xml->vertices[xml->triind[a+((j+2)%3)]].z)-Cur)
                      .Cross(Vector (xml->vertices[xml->triind[a+((j+1)%3)]].x,
                                     xml->vertices[xml->triind[a+((j+1)%3)]].y,
                                     xml->vertices[xml->triind[a+((j+1)%3)]].z)-Cur);
                  Normalize(Cur);
                  //Cur = Cur*(1.00F/xml->vertexcount[a+j]);
                  xml->tris[a+j].i=Cur.i;
                  xml->tris[a+j].j=Cur.j;
                  xml->tris[a+j].k=Cur.k;
#if 0
                  xml->tris[a+j].i=Cur.i/xml->vertexcount[xml->triind[a+j]];
                  xml->tris[a+j].j=Cur.j/xml->vertexcount[xml->triind[a+j]];
                  xml->tris[a+j].k=Cur.k/xml->vertexcount[xml->triind[a+j]];
#endif
              }
          }
      }
      a=0;
      trimax = xml->quads.size()/4;
      for (i=0;i<trimax;i++,a+=4) {
          if (xml->quadshade[i]==1||(FLAT_SHADE)) {
              for (j=0;j<4;j++) {
                  Vector Cur (xml->vertices[xml->quadind[a+j]].x,
                              xml->vertices[xml->quadind[a+j]].y,
                              xml->vertices[xml->quadind[a+j]].z);
                  Cur = (Vector (xml->vertices[xml->quadind[a+((j+2)%4)]].x,
                                 xml->vertices[xml->quadind[a+((j+2)%4)]].y,
                                 xml->vertices[xml->quadind[a+((j+2)%4)]].z)-Cur)
                      .Cross(Vector (xml->vertices[xml->quadind[a+((j+1)%4)]].x,
                                     xml->vertices[xml->quadind[a+((j+1)%4)]].y,
                                     xml->vertices[xml->quadind[a+((j+1)%4)]].z)-Cur);
                  Normalize(Cur);
                  //Cur = Cur*(1.00F/xml->vertexcount[a+j]);
                  xml->quads[a+j].i=Cur.i;//xml->vertexcount[xml->quadind[a+j]];
                  xml->quads[a+j].j=Cur.j;//xml->vertexcount[xml->quadind[a+j]];
                  xml->quads[a+j].k=Cur.k;//xml->vertexcount[xml->quadind[a+j]];
                  
#if 0
                  xml->quads[a+j].i=Cur.i/xml->vertexcount[xml->quadind[a+j]];
                  xml->quads[a+j].j=Cur.j/xml->vertexcount[xml->quadind[a+j]];
                  xml->quads[a+j].k=Cur.k/xml->vertexcount[xml->quadind[a+j]];
#endif
              }
          }
      }
      
  }
  string factionname = FactionUtil::GetFaction(xml->faction);
  while (Decal.size()<xml->decals.size())
      Decal.push_back(NULL);
  Decal[0]=(TempGetTexture(0,factionname));
  {for (unsigned int i=1;i<xml->decals.size();i++) {
      Decal[i]=(TempGetTexture(i,factionname));
  }}
  while (Decal.back()==NULL&&Decal.size()>1) {
      Decal.pop_back();
  }

  unsigned int index = 0;

  unsigned int totalvertexsize = xml->tris.size()+xml->quads.size()+xml->lines.size();
  for (index=0;index<xml->tristrips.size();index++) {
    totalvertexsize += xml->tristrips[index].size();
  }
  for (index=0;index<xml->trifans.size();index++) {
    totalvertexsize += xml->trifans[index].size();
  }
  for (index=0;index<xml->quadstrips.size();index++) {
    totalvertexsize += xml->quadstrips[index].size();
  }
  for (index=0;index<xml->linestrips.size();index++) {
    totalvertexsize += xml->linestrips[index].size();
  }

  index =0;
  GFXVertex *vertexlist = new GFXVertex[totalvertexsize];

  mn = Vector (FLT_MAX,FLT_MAX,FLT_MAX);
  mx = Vector (-FLT_MAX,-FLT_MAX,-FLT_MAX);
  radialSize = 0;
  vector <enum POLYTYPE> polytypes;
  polytypes.insert(polytypes.begin(),totalvertexsize,GFXTRI);
  //  enum POLYTYPE * polytypes= new enum POLYTYPE[totalvertexsize];//overkill but what the hell
  vector <int> poly_offsets;
  poly_offsets.insert (poly_offsets.begin(),totalvertexsize,0);
  int o_index=0;
  if (xml->tris.size()) {
    polytypes[o_index]= GFXTRI;
    poly_offsets[o_index]=xml->tris.size();
    o_index++;
  }
  if (xml->quads.size()) {
    polytypes[o_index]=GFXQUAD;
    poly_offsets[o_index]=xml->quads.size();
    o_index++;
  }
  if (xml->lines.size()) {
    polytypes[o_index]=GFXLINE;
    poly_offsets[o_index]=xml->lines.size();
    o_index++;
  }

  for(a=0; a<xml->tris.size(); a++, index++) {
    vertexlist[index] = xml->tris[a];		
    updateMax (mn,mx,vertexlist[index]);
  }
  for(a=0; a<xml->quads.size(); a++, index++) {
    vertexlist[index] = xml->quads[a];
    updateMax (mn,mx,vertexlist[index]);
  }
  for(a=0; a<xml->lines.size(); a++, index++) {
    vertexlist[index] = xml->lines[a];		
    updateMax (mn,mx,vertexlist[index]);
  }

  for (a=0;a<xml->tristrips.size();a++) {

    for (unsigned int m=0;m<xml->tristrips[a].size();m++,index++) {
      vertexlist[index] = xml->tristrips[a][m];
    updateMax (mn,mx,vertexlist[index]);
    }
    polytypes[o_index]= GFXTRISTRIP;
    poly_offsets[o_index]=xml->tristrips[a].size();
    o_index++;
  }
  for (a=0;a<xml->trifans.size();a++) {
    for (unsigned int m=0;m<xml->trifans[a].size();m++,index++) {
      vertexlist[index] = xml->trifans[a][m];
    updateMax (mn,mx,vertexlist[index]);
    }
    polytypes[o_index]= GFXTRIFAN;
    poly_offsets[o_index]=xml->trifans[a].size();

    o_index++;
  }
  for (a=0;a<xml->quadstrips.size();a++) {
    for (unsigned int m=0;m<xml->quadstrips[a].size();m++,index++) {
      vertexlist[index] = xml->quadstrips[a][m];
    updateMax (mn,mx,vertexlist[index]);
    }
    polytypes[o_index]= GFXQUADSTRIP;
    poly_offsets[o_index]=xml->quadstrips[a].size();
    o_index++;
  }
  for (a=0;a<xml->linestrips.size();a++) {

    for (unsigned int m=0;m<xml->linestrips[a].size();m++,index++) {
      vertexlist[index] = xml->linestrips[a][m];
      updateMax (mn,mx,vertexlist[index]);
    }
    polytypes[o_index]= GFXLINESTRIP;
    poly_offsets[o_index]=xml->linestrips[a].size();
    o_index++;
  }
  if (mn.i==FLT_MAX&&mn.j==FLT_MAX&&mn.k==FLT_MAX) {
	  mx.i=mx.j=mx.k=mn.i=mn.j=mn.k=0;
  }
  mn.i *=xml->scale.i;
  mn.j *=xml->scale.j;
  mn.k *=xml->scale.k;
  mx.i *=xml->scale.i;
  mx.j *=xml->scale.j;
  mx.k *=xml->scale.k;  
  float x_center = (mn.i + mx.i)/2.0,
    y_center = (mn.j + mx.j)/2.0,
    z_center = (mn.k + mx.k)/2.0;
  local_pos = Vector (x_center, y_center, z_center);
  for(a=0; a<totalvertexsize; a++) {
    vertexlist[a].x*=xml->scale.i;//FIXME
    vertexlist[a].y*=xml->scale.j;
    vertexlist[a].z*=xml->scale.k;
    vertexlist[a].i*=-1;
    vertexlist[a].j*=-1;
    vertexlist[a].k*=-1;


  }
  for (a=0;a<xml->vertices.size();a++) {
    xml->vertices[a].x*=xml->scale.i;//FIXME
    xml->vertices[a].y*=xml->scale.j;
    xml->vertices[a].z*=xml->scale.k;
    xml->vertices[a].i*=-1;    xml->vertices[a].k*=-1;    xml->vertices[a].j*=-1;
  }
  if( o_index || index)
 	 radialSize = .5*(mx-mn).Magnitude();

  if (xml->sharevert) {
    vlist = new GFXVertexList (&polytypes[0], xml->vertices.size(),&xml->vertices[0],o_index,&poly_offsets[0],false,&ind[0]);
  }else {
    static bool usopttmp=(XMLSupport::parse_bool (vs_config->getVariable ("graphics","OptimizeVertexArrays","true")));
    static float optvertexlimit= (XMLSupport::parse_float (vs_config->getVariable ("graphics", "OptimizeVertexCondition","1.0")));
    bool cachunk=false;
    if (usopttmp) {
      int numopt =totalvertexsize;      
      GFXVertex * newv;
      unsigned int * ind;
      GFXOptimizeList (vertexlist,totalvertexsize,&newv,&numopt,&ind);
      if (numopt < totalvertexsize*optvertexlimit) {
	vlist = new GFXVertexList (&polytypes[0], numopt,newv,o_index,&poly_offsets[0],false,ind);
	cachunk = true;
      }
      free (ind);
      free (newv);
    }
    if (!cachunk) {
      vlist= new GFXVertexList(&polytypes[0],totalvertexsize,vertexlist,o_index,&poly_offsets[0]); 
    }
  }
  CreateLogos(faction,fg);
  // Calculate bounding sphere
  
  if (mn.i==FLT_MAX) {
    mn=Vector (0,0,0);
    mx=Vector (0,0,0);
  }

  GFXSetMaterial (myMatNum,xml->material);

  delete [] vertexlist;
  numlods=xml->lod.size()+1;
  if (origthis) {
    orig=NULL;
  }else {
    orig = new Mesh [numlods];
    for (i=0;i<xml->lod.size();i++) {
      orig[i+1] = *xml->lod[i];
      orig[i+1].lodsize=xml->lodsize[i];
    } 
  }
  delete xml;
}
*/