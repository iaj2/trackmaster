/* 
GlobalEntities.h
--------------------------------------------------------------------------------------------------------------------
Rev. 1 - 2024/07/23 Original by Isaac James
--------------------------------------------------------------------------------------------------------------------
This header file declares global instances of the EntityIO class template for different entity types.
These global instances are used to perform I/O operations on various entities throughout the application.
*/


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