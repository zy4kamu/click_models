/** 
 * @file	SparseVector.h
 * @brief	header file of the class containing sequence of labeled doubles 
 *
 * Copyright 2013 by Samsung Electronics, Inc.,
 * 
 * This software is the confidential and proprietary information
 * of Samsung Electronics, Inc. ("Confidential Information").  You
 * shall not disclose such Confidential Information and shall use
 * it only in accordance with the terms of the license agreement
 * you entered into with Samsung.
 */

#ifndef SPARSE_VECTOR
#define SPARSE_VECTOR

#include <vector>
#include <memory>
#include <map>

using std::vector;
using std::shared_ptr;
using std::map;

namespace Tools
{

/** 
 * @class	SparseVector
 * @brief	contains sequence of labeled doubles 
 *
 * Copyright 2013 by Samsung Electronics, Inc.,
 * 
 * This software is the confidential and proprietary information
 * of Samsung Electronics, Inc. ("Confidential Information").  You
 * shall not disclose such Confidential Information and shall use
 * it only in accordance with the terms of the license agreement
 * you entered into with Samsung.
 */
class SparseVector
{
public:
    /**
     * @brief	Constructor
     */
    SparseVector();

    /**
     * @brief	Constructor
     * @param[in]	data - map that contains indexes and values
     */
    SparseVector(const map<int, double>& data);

	/**
     * @brief	Constructor
	 * @param[in]	_indexes - labels of the SparseVector (_indexes must be sorted)
	 * @param[in]	_valued - data
     */
	SparseVector(shared_ptr<vector<int>> _indexes, shared_ptr<vector<double>> _values);

    /**
     * @brief	Copy constructor
     * @param[in]	data - sparse vector to copy
     */
    SparseVector(const SparseVector& data);

    /**
     * @brief	Move constructor
     * @param[in]	data - sparse vector to move
     */
    SparseVector(SparseVector&& data);

    /**
     * @brief	Assignment operator
     * @param[in]	data - sparse vector to copy
     */
    SparseVector& operator = (const SparseVector& data);

    /**
     * @brief	Assignment operator
     * @param[in]	data - sparse vector to move
     */
    SparseVector& operator = (SparseVector&& data);

	/**
     * @brief	Destructor
     */
	~SparseVector(void);

	/**
     * @brief	Returns the number of elements in SparseVector
     */
	int Size() const;

	/**
     * @brief	Returns value with corresponding index 
	 * @param[in]	index - index of the value
     */
	double GetValue(int index) const;

	/**
     * @brief	Returns the label with the corresponding index
	 * @param[in]	index - index
     */
	double GetIndex(int index) const;

	/**
     * @brief	Returns the index of the corresponding label if indexes contains it and return -1 otherwise
	 * @param[in]	index - index
     */
	int Contains(int index) const;

	/**
     * @brief	Returns the sequence of labels
     */
	const vector<int>& GetIndexes() const;

	/**
     * @brief	Returns the sequence of values
     */
	const vector<double>& GetValues() const;

     /**
     * @brief	Removes all features with weight smaller than trancationLevel
     * @param[in]	truncationLevel - Removes all entries with weight smaller than trancationLevel
     */
    void Truncate(double truncationLevel);

    /**
    * @brief	Converts sparse vector to map
    */
    map<int, double> ConvertToMap();
private:
	shared_ptr<vector<int> > indexes; /**< labels*/
	shared_ptr<vector<double> > values; /**< data*/
};

}

#endif // SPARSE_VECTOR

