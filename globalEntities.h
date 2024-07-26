#ifndef GLOBALENTITIES_H
#define GLOBALENTITIES_H

#include "EntityIO.h"
#include "Product.h"
#include "ProductRelease.h"
#include "Request.h"
#include "Requester.h"
#include "Change.h"

extern EntityIO<Product> productIO;
extern EntityIO<ProductRelease> productReleaseIO;
extern EntityIO<Request> requestIO;
extern EntityIO<Requester> requesterIO;
extern EntityIO<Change> changeIO;

#endif // GLOBALENTITIES_H