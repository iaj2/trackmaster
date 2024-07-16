#include "Change.cpp";
#include "Requester.h";
#include "Request.h";
#include "Product.h";
#include "ProductRelease.h";
#include "LifeCycleController.h";

void LifeCycleController::startUpSystem() {
  // init program entities
  ProductRelease::initProductRelease();
  Product::initProduct();
  Request::initRequest();
  Change::initChange();
  Requester::initRequester();
}

void LifeCycleController::shutDownSystem() {
  // close program entities
  ProductRelease::exitProductRelease();
  Product::exitProduct();
  Request::exitRequest();
  Change::exitChange();
  Requester::exitRequester();
}