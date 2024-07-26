<<<<<<< HEAD
#include "Change.h";
#include "Requester.h";
#include "Request.h";
#include "Product.h";
#include "ProductRelease.h";
#include "LifeCycleController.h";
#include "EntityIO.h";
=======
#include "Change.cpp"
#include "Requester.h"
#include "Request.h"
#include "Product.h"
#include "ProductRelease.h"
#include "LifeCycleController.h"
#include "EntityIO.h"
#include "globalEntities.h"
>>>>>>> 7e92c730d730683e813e5f95d8e4e61f006d54a7

EntityIO<Product> productIO("product.dat");
EntityIO<ProductRelease> productReleaseIO("productRelease.dat");
EntityIO<Request> requestIO("request.dat");
EntityIO<Requester> requesterIO("requester.dat");
EntityIO<Change> changeIO("change.dat");

void LifeCycleController::startUpSystem() {
  // init program entities
  productIO.init();
  productReleaseIO.init();
  requestIO.init();
  requesterIO.init();
  changeIO.init();
}

void LifeCycleController::shutDownSystem() {
  // close program entities
  productIO.exit();
  productReleaseIO.exit();
  requestIO.exit();
  requesterIO.exit();
  changeIO.exit();
}