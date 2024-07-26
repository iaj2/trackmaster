#include "Change.h"
#include "Requester.h"
#include "Request.h"
#include "Product.h"
#include "ProductRelease.h"
#include "LifeCycleController.h"
#include "EntityIO.h"
#include "globalEntities.h"

EntityIO<Product> productIO("product.dat");
EntityIO<ProductRelease> productReleaseIO("productRelease.dat");
EntityIO<Request> requestIO("request.dat");
EntityIO<Requester> requesterIO("requester.dat");
EntityIO<Change> changeIO("change.dat");

namespace LifeCycleController {
  void startUpSystem() {
    // init program entities
    productIO.init();
    productReleaseIO.init();
    requestIO.init();
    requesterIO.init();
    changeIO.init();
  }

  void shutDownSystem() {
    // close program entities
    productIO.exit();
    productReleaseIO.exit();
    requestIO.exit();
    requesterIO.exit();
    changeIO.exit();
  }
}
