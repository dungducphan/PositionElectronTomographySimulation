#include "PETDetectorConstruction.hh"
#include "PETDetectorMessenger.hh"
#include "PETTrackerSD.hh"

#include "G4SDManager.hh"
#include "G4PhysicalConstants.hh"
#include "G4OpticalSurface.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4GeometryManager.hh"
#include "G4VisAttributes.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"

#include <iostream>
#include <fstream>
#include <vector>

PETDetectorConstruction::PETDetectorConstruction()
: G4VUserDetectorConstruction(),
  world_hx(5.0 * m),
  world_hy(5.0 * m),
  world_hz(5.0 * m),
  fMessenger(NULL),
  fLYSO_MPT(NULL),
  fAir_MPT(NULL),
  fOpeningAngle(1.5),
  fInnerDiameter(50*cm),
  fOuterDiameter(60*cm),
  fHeight(10*cm),
  worldLog(NULL),
  worldPhy(NULL),
  logicCryst(NULL) {
  fMessenger = new PETDetectorMessenger(this);
}

PETDetectorConstruction::~PETDetectorConstruction() {
}

void PETDetectorConstruction::BuildMaterial() {
  G4NistManager * nist = G4NistManager::Instance();

  G4Element *Oxigen = nist->FindOrBuildElement("O");
  G4Element *Lutetium = nist->FindOrBuildElement("Lu");
  G4Element *Yttrium = nist->FindOrBuildElement("Y");

  G4Element *fTeflon = nist->FindOrBuildElement("Teflon");

  G4Element *Na = nist->FindOrBuildElement("Na");
  G4Element *I = nist->FindOrBuildElement("I");

  G4Material* fWater = G4NistManager::Instance()->FindOrBuildMaterial("G4_WATER");

  // Air
  fAir = nist->FindOrBuildMaterial("G4_AIR");

  //NaI
  fNaI = new G4Material("SodiumIodine", 3.67*g/cm3, 2);
  fNaI->AddElement(Na,1);
  fNaI->AddElement(I,1);

  // Lu2O3
  fLutetiumOxide = new G4Material("LutetiumOxide", 9.41 * g / cm3, 2);
  fLutetiumOxide->AddElement(Lutetium, 2);
  fLutetiumOxide->AddElement(Oxigen, 3);

  // SiO2
  fSiliconDioxide = nist->FindOrBuildMaterial("G4_SILICON_DIOXIDE");

  //Silica Gel
  fGel = new G4Material("Gel", 2.21 * g/cm3, 1);
  fGel->AddMaterial(fSiliconDioxide, 100* perCent);

  // Y2O3
  fYttriumOxide = new G4Material("YttriumOxide", 5.01 * g / cm3, 2);
  fYttriumOxide->AddElement(Yttrium, 2);
  fYttriumOxide->AddElement(Oxigen, 3);

  // LYSO
  fLYSO = new G4Material("LYSO", 7.1 * g / cm3, 3);
  fLYSO->AddMaterial(fLutetiumOxide, 81 * perCent);
  fLYSO->AddMaterial(fSiliconDioxide, 14 * perCent);
  fLYSO->AddMaterial(fYttriumOxide, 5 * perCent);

  // Optical Properties of Materials ( nm/1242.38 )
  const G4int num = 120;
  
  G4double ene[num] = {3.51370304301*eV,3.46284728249*eV,3.4156577087*eV,3.36977603198*eV,3.23635193804*eV,3.22094763375*eV,3.20764136152*eV,3.19637713232*eV,3.18291759837*eV,3.17218298275*eV,3.16123184911*eV,3.1503731311*eV,3.1396057038*eV,3.12928927829*eV,3.12192642432*eV,3.11495009612*eV,3.10797996538*eV,3.10108780463*eV,3.09434823581*eV,3.0876570207*eV,3.0810055618*eV,3.07438540703*eV,3.06780442816*eV,3.06124350731*eV,3.05469721997*eV,3.04817354563*eV,3.04165911826*eV,3.03518039566*eV,3.02874761411*eV,3.02234465979*eV,3.01597393385*eV,3.00960145787*eV,3.00323000843*eV,2.99723759298*eV,2.99057154424*eV,2.984211427*eV,2.97752386539*eV,2.9682041556*eV,2.95859140503*eV,2.95063652972*eV,2.93947102052*eV,2.92513342795*eV,2.89963655932*eV,2.87253927733*eV,2.85352309132*eV,2.83628914618*eV,2.8222957219*eV,2.80993218523*eV,2.79766640437*eV,2.78549390188*eV,2.77344008928*eV,2.76149998282*eV,2.74965574426*eV,2.73791911725*eV,2.72625989445*eV,2.71468687979*eV,2.70321484598*eV,2.62319693516*eV,2.61249533729*eV,2.60188652089*eV,2.59136351654*eV,2.58094246644*eV,2.5706077365*eV,2.56035262437*eV,2.55018739616*eV,2.53884632056*eV,2.46805717364*eV,2.45631868318*eV,2.44469337969*eV,2.43204791693*eV,2.41841286976*eV,2.40494311619*eV,2.38943563283*eV,2.37197339274*eV,2.35478359973*eV,2.33681395534*eV,2.31707351876*eV,2.30377129545*eV,2.25609178885*eV,2.23488272047*eV,2.21410318259*eV,2.19371928099*eV,2.17372057346*eV,2.15408900424*eV,2.13481455788*eV,2.11588408116*eV,2.09729119825*eV,2.07902560898*eV,2.0610754277*eV,2.04343516532*eV,2.02609686477*eV,2.00905157727*eV,1.99229317822*eV,1.97581265517*eV,1.95960675492*eV,1.94367103348*eV,1.92800809024*eV,1.91260128415*eV,1.89744776279*eV,1.88252859884*eV,1.86783894415*eV,1.85337461802*eV,1.83912783245*eV,1.82509840454*eV,1.81128037215*eV,1.7976639421*eV,1.78425469025*eV,1.77643183324*eV,2.64723791997*eV,2.65431419592*eV,2.66836164719*eV,2.6790662172*eV,2.6863139081*eV,2.69716329233*eV,2.53364876322*eV,2.5209805194*eV,2.51147200842*eV,2.49905148118*eV,2.48976188644*eV,2.48661723357*eV};

  G4double fast[num] = {0.013820781627794876, 0.013938358896580905, 0.014050591744058316, 0.01770083149773294, 0.1361291993771121, 0.17951253934849465, 0.22274311794414414, 0.2652312673463606, 0.3114997040874239, 0.35139224620750076, 0.40066157551326187, 0.4516999082721216, 0.5045072444840795, 0.5579031796294753, 0.6034774651162591, 0.6472816782656827, 0.688432386235458, 0.7345952706556798, 0.7938782640197157, 0.8552250947457001, 0.9177512611070833, 0.9805722613773165, 1.0445725972829487, 1.1076884314620314, 1.1693300960968656, 1.2303820929140001, 1.2893702523691863, 1.3492429135509216, 1.4111794120946055, 1.4734107445471392, 1.5362317448173723, 1.5958095720902579, 1.6524390602746462, 1.7103057819919416, 1.766140287506697, 1.8148092601521415, 1.8614449477107844, 1.910241473878124, 1.9568572980042371, 1.9966865978054973, 2.040506844218846, 2.084160879129502, 2.1189188575511695, 2.0948333321877324, 2.053620718004467, 2.012992427217593, 1.9736802001779261, 1.9334781269904011, 1.8919493012130524, 1.848651471982605, 1.807122646205256, 1.7669205730177313, 1.7258339981036572, 1.6856319249161322, 1.642334095685685, 1.597267263002139, 1.5526426811818679, 1.2800838749773036, 1.2372282966101307, 1.1952572199695073, 1.153286143328884, 1.1139685718679084, 1.0750932512702074, 1.035775679809232, 0.9977848609380802, 0.9548461768195611, 0.6689313945441024, 0.6314819655514974, 0.594386337249512, 0.556352584860516, 0.5169089741303492, 0.4798240346709801, 0.4399488619201559, 0.3975782897867255, 0.3587457245594927, 0.32193320213070775, 0.2849053292144217, 0.2621546621464601, 0.2066071763487929, 0.17922479084668286, 0.1590792376527035, 0.1416675988862397, 0.12715069304302862, 0.11392033516570743, 0.1019765252542757, 0.09051517083005267, 0.08017954587598375, 0.07064801340059512, 0.061116480925207384, 0.05222822243276415, 0.043983237923266305, 0.03605989040524049, 0.028779816870159625, 0.02166056183081544, 0.015667036261624467, 0.011442514145532101, 0.01156009141431813, 0.013285853640466527, 0.017584711798394714, 0.020757840486169243, 0.02296605819952635, 0.024531001929938068, 0.024648579198724097, 0.024766156467510125, 0.02456209674482368, 0.022428215073302393, 0.021580881367671, 0.023419362297779855, 1.3330963870244557, 1.3692671524065998, 1.4099353409976352, 1.4506171995680286, 1.486787964950173, 1.5274698235205666, 0.9082334285675229, 0.8675652399764877, 0.8223586182384865, 0.7862151928150589, 0.750058097412273, 0.727447951553593};

  G4double rLyso[num] = {1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82};
  
  G4double rAir[num] = {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0};

  G4double abs[num] = {1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm,1.2*cm};

  fLYSO_MPT = new G4MaterialPropertiesTable();
  fAir_MPT = new G4MaterialPropertiesTable();
  fGel_MPT = new G4MaterialPropertiesTable();
  


  //Rise and Decay Time  https://iopscience.iop.org/article/10.1088/1748-0221/7/09/P09004/pdf 

  fLYSO_MPT->AddProperty("FASTCOMPONENT", ene, fast, num)->SetSpline(true);
  fLYSO_MPT->AddProperty("RINDEX", ene, rLyso, num);
  fLYSO_MPT->AddProperty("ABSLENGTH", ene, abs, num);
  fLYSO_MPT->AddConstProperty("SCINTILLATIONYIELD", 30000 / MeV);
  fLYSO_MPT->AddConstProperty("RESOLUTIONSCALE", 1);
  fLYSO_MPT->AddConstProperty("FASTTIMECONSTANT", 43 * ns);
  fLYSO_MPT->AddConstProperty("RISETIMECONSTANT", 72 * ps);

  fAir_MPT->AddProperty("RINDEX", ene, rAir, num);

  fGel_MPT->AddProperty("RINDEX", ene, rLyso, num);



  fLYSO->SetMaterialPropertiesTable(fLYSO_MPT);
  fAir->SetMaterialPropertiesTable(fAir_MPT);
  fGel->SetMaterialPropertiesTable(fGel_MPT);


}

G4VPhysicalVolume * PETDetectorConstruction::Construct() {
  BuildMaterial();
  
  // Overlapping check
  G4bool checkOverlaps = true;

  // Clear old geometry if any
  if (worldPhy) {
      G4GeometryManager::GetInstance()->OpenGeometry();
      G4PhysicalVolumeStore::GetInstance()->Clean();
      G4LogicalVolumeStore::GetInstance()->Clean();
      G4SolidStore::GetInstance()->Clean();
  }

  worldBox = new G4Box("WorldBox", world_hx, world_hy, world_hz);
  worldLog = new G4LogicalVolume(worldBox, fAir, "WorldLog");
  worldPhy = new G4PVPlacement(0, G4ThreeVector(), worldLog, "WorldPhy", 0, false, 0, checkOverlaps);

  // waterBox = new G4Box("WaterBox", 5*mm, 5*mm, 5*mm);
  // waterLog = new G4LogicalVolume(waterBox, fWater, "WaterLog");
  // waterPhy = new G4PVPlacement(0, G4ThreeVector(0,0,0), waterLog, "WaterPhy", worldLog, false, checkOverlaps);

  
  G4double cryst_dX = 30*mm, cryst_dY = 30*mm, cryst_dZ = 30*mm, det_dZ = 1*mm;
  G4int nb_cryst = 32;
  G4double dPhi = twopi/nb_cryst, half_dPhi = 0.5*dPhi;
  G4double cosdPhi = std::cos(half_dPhi);
  G4double tandPhi = std::tan(half_dPhi);
  G4double ring_R1 = 0.5*cryst_dY/tandPhi;
  G4double ring_R2 = (ring_R1+cryst_dZ)/cosdPhi;

  std::cout<<ring_R1<<std::endl;


  G4double gap = 0.2*mm;
  G4double dX = cryst_dX - gap, dY = cryst_dY - gap;

  solidCryst = new G4Box("crystal", dX/2, dY/2, cryst_dZ/2);
  logicCryst = new G4LogicalVolume(solidCryst, fLYSO, "CrystalLV");

  solidDet_outter = new G4Box("detector", dX/2, dY/2, det_dZ/2);
  solidDet_inner = new G4Box("detector", dX/2, dY/2, det_dZ/2);
  logicDet_outter = new G4LogicalVolume(solidDet_outter, fAir, "DetectorLV");
  logicDet_inner = new G4LogicalVolume(solidDet_outter, fAir, "DetectorLV");

  
  G4double pDx1= dX/2, pDx2=dX/2, pDz= (cryst_dZ)/2, pTheta =0, pPhi =0, pDy1 = gap/2, pDy2 = (5.8*mm)/2, pDx3 = (cryst_dZ+gap)/2, pDx4 = (cryst_dZ+gap)/2, pAlp1 = 0*degree,pAlp2 = 0*degree ;
  
  solidGel = new G4Trap("gel", pDz, pTheta, pPhi, pDy1, pDx1, pDx2, pAlp1, pDy2, pDx3, pDx4, pAlp2);
  //  solidGel = new G4Trap("gel", cryst_dZ+2*det_dZ, dY, 2*cryst_dZ+gap, gap);
  logicGel = new G4LogicalVolume(solidGel, fGel, "GelLV");
  //  solidGel = new G4Tubs("gel", ring_R1, ring_R2+det_dZ, cryst_dX/2, 0, 2*gap/ring_R2);//-2*gap/ring_R2, 2*gap/ring_R2);
  //  G4Tubs* solidDet_outter = new G4Tubs("detector", ring_R2, ring_R2+det_dX, 0.5*cryst_dX, 0, twopi/(nb_cryst+gap));
  //G4Tubs* solidDet_inner = new G4Tubs("detector", ring_R1-det_dX, ring_R1, 0.5*cryst_dX, 0, twopi/(nb_cryst+gap));
  //G4LogicalVolume* logicDet_outter =
  //new G4LogicalVolume(solidDet_outter,
  //                    fAir,
  //                    "DetectorLV");
  //G4LogicalVolume* logicDet_inner =
  //new G4LogicalVolume(solidDet_inner,
  //			fAir,
  //			"DetectorLV");
  for (G4int icrys = 0; icrys < nb_cryst ; icrys++) {
    G4double phi = icrys*dPhi;
    G4double gphi = icrys*dPhi+half_dPhi;
    G4RotationMatrix rotm = G4RotationMatrix();
    rotm.rotateY(90*degree);
    rotm.rotateZ(phi);
    
    G4RotationMatrix rotd = G4RotationMatrix();
    rotd.rotateY(90*degree);
    rotd.rotateZ(phi);
    
    G4RotationMatrix rotg = G4RotationMatrix();
    rotg.rotateY(90*degree);
    rotg.rotateZ(gphi);
   
    G4ThreeVector uz = G4ThreeVector(std::cos(phi),  std::sin(phi), 0);
    G4ThreeVector ug = G4ThreeVector(std::cos(gphi),  std::sin(gphi), 0);
    G4ThreeVector position_cryst = (ring_R1+0.5*cryst_dZ)*uz;
    G4ThreeVector position_det_outter = (ring_R2-0.5*det_dZ)*uz;
    G4ThreeVector position_det_inner = (ring_R1-0.5*det_dZ)*uz;
    G4ThreeVector position_gel = (ring_R1+0.5*cryst_dZ)*ug;
    G4Transform3D transform_cryst = G4Transform3D(rotm,position_cryst);
    G4Transform3D transform_det_outter = G4Transform3D(rotm,position_det_outter);
    G4Transform3D transform_det_inner = G4Transform3D(rotm,position_det_inner);
    G4Transform3D transform_gel = G4Transform3D(rotg,position_gel);
    G4String detector = "detector";
    new G4PVPlacement(transform_cryst,             //rotation,position
    		      logicCryst,            //its logical volume
    		      "crystal",             //its name
    		      worldLog,//logicRing,             //its mother  volume
    		      false,                 //no boolean operation
    		      icrys,                 //copy number
    		      checkOverlaps);       // checking overlaps
    new G4PVPlacement(transform_det_outter,             //rotation,position
    		      logicDet_outter,            //its logical volume
    		      "detector",             //its name
    		      worldLog,//logicRing,             //its mother  volume
    		      false,                 //no boolean operation
    		      icrys,                 //copy number
    		      checkOverlaps);       // checking overlaps
    new G4PVPlacement(transform_det_inner,             //rotation,position
    		      logicDet_inner,            //its logical volume
    		      "detector",             //its name
    		      worldLog,//logicRing,             //its mother  volume
    		      false,                 //no boolean operation
    		      icrys+nb_cryst,                 //copy number
    		      checkOverlaps);       // checking overlaps
    new G4PVPlacement(transform_gel,
    		      logicGel,
    		      "gel",
    		      worldLog,
    		      false,
    		      icrys,
    		      checkOverlaps);

  }
  

  G4VisAttributes* crystVisAttributes = new G4VisAttributes(G4Colour(1.0,1.0,1.0));
  G4VisAttributes* SiPMVisAttributes = new G4VisAttributes(G4Colour(0.0,1.0,0.0));
  G4VisAttributes* GelVisAttributes = new G4VisAttributes(G4Colour(1.0,0.0,0.0));

  logicGel->SetVisAttributes(GelVisAttributes);

  logicDet_outter->SetVisAttributes(SiPMVisAttributes);
  logicDet_inner->SetVisAttributes(SiPMVisAttributes);



  // G4OpticalSurface* opDetectorSurface = new G4OpticalSurface("DetectorSurface");
  // //  opDetectorSurface->SetType(dielectric_metal);
  // opDetectorSurface->SetFinish(ground);
  // //opDetectorSurface->SetModel(glisur);
  // G4LogicalSkinSurface* detectorSurface_inner = new G4LogicalSkinSurface("DetectorSurface", logicDet_inner, opDetectorSurface);
  // G4LogicalSkinSurface* detectorSurface_outter = new G4LogicalSkinSurface("DetectorSurface", logicDet_outter, opDetectorSurface);


  G4double ephoton_pmt[4]      = {0.0001*eV, 1*eV, 10*eV, 100*eV};
  G4double reflectivity_pmt[4] = {0,0,0,0};
  G4double efficiency_pmt[4]   = {1.0,1.0,1.0,1.0};

  // G4MaterialPropertiesTable *detectorMPT = new G4MaterialPropertiesTable();
  // detectorMPT->AddProperty("REFLECTIVITY", ephoton_pmt, reflectivity_pmt, 4);
  // detectorMPT->AddProperty("EFFICIENCY",   ephoton_pmt, efficiency_pmt,   4);
  // opDetectorSurface->SetMaterialPropertiesTable(detectorMPT);



  return worldPhy;
}

void PETDetectorConstruction::SetOpeningAngle(G4double newValue) {
    this->fOpeningAngle = newValue;
    G4RunManager::GetRunManager()->DefineWorldVolume(Construct());
}

void PETDetectorConstruction::SetInnerDiameter(G4double newValue) {
    this->fInnerDiameter = newValue;
    G4RunManager::GetRunManager()->DefineWorldVolume(Construct());
}

void PETDetectorConstruction::SetOuterDiameter(G4double newValue) {
    this->fOuterDiameter = newValue;
    G4RunManager::GetRunManager()->DefineWorldVolume(Construct());
}

void PETDetectorConstruction::SetHeight(G4double newValue) {
    this->fHeight = newValue;
    G4RunManager::GetRunManager()->DefineWorldVolume(Construct());
}



void PETDetectorConstruction::ConstructSDandField() {
  // Sensitive detectors

  PETTrackerSD* aTrackerSD = new PETTrackerSD("TrackerChamberSD");
  G4SDManager::GetSDMpointer()->AddNewDetector(aTrackerSD);

  // Setting aTrackerSD to all logical volumes with the same name of "Chamber_LV".
  SetSensitiveDetector("DetectorLV", aTrackerSD, true);
}

