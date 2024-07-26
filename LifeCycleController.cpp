#include "Change.cpp";
#include "Requester.h";
#include "Request.h";
#include "Product.h";
#include "ProductRelease.h";
#include "LifeCycleController.h";
#include "EntityIO.h";

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