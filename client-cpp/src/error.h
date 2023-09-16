//
// Created by Peyton Seigo on 2023-09-15.
//

#ifndef CRDTDEMOS_ERROR_H
#define CRDTDEMOS_ERROR_H

enum class ResourceError {
    /// Data and its schema do not match.
    DATA_SCHEMA_MISMATCH,

    DATA_MISSING_FIELD,

};

#endif //CRDTDEMOS_ERROR_H
