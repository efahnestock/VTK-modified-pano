// SPDX-FileCopyrightText: Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
// SPDX-License-Identifier: BSD-3-Clause
/**
 * @class   vtkHyperTreeGridThreshold
 * @brief   Extract cells from a hyper tree grid
 * where selected scalar value is within given range.
 *
 *
 * This filter extracts cells from a hyper tree grid that satisfy the
 * following threshold: a cell is considered to be within range if its
 * value for the active scalar is within a specified range (inclusive).
 * The output remains a hyper tree grid.
 * JB Un parametre (JustCreateNewMask=true) permet de ne pas faire
 * le choix de la creation d'un nouveau HTG mais
 * de redefinir juste le masque.
 *
 * @sa
 * vtkHyperTreeGrid vtkHyperTreeGridAlgorithm vtkThreshold
 *
 * @par Thanks:
 * This class was written by Guenole Harel and Jacques-Bernard Lekien 2014
 * This class was revised by Philippe Pebay, 2016
 * This class was optimized by Jacques-Bernard Lekien, 2018.
 * This work was supported by Commissariat a l'Energie Atomique
 * CEA, DAM, DIF, F-91297 Arpajon, France.
 */

#ifndef vtkHyperTreeGridThreshold_h
#define vtkHyperTreeGridThreshold_h

#include "vtkDeprecation.h"            // For deprecation macros
#include "vtkFiltersHyperTreeModule.h" // For export macro
#include "vtkHyperTreeGridAlgorithm.h"

#include <memory> // For std::unique_ptr

VTK_ABI_NAMESPACE_BEGIN
class vtkBitArray;
class vtkHyperTreeGrid;

class vtkHyperTreeGridNonOrientedCursor;

class VTKFILTERSHYPERTREE_EXPORT vtkHyperTreeGridThreshold : public vtkHyperTreeGridAlgorithm
{
public:
  static vtkHyperTreeGridThreshold* New();
  vtkTypeMacro(vtkHyperTreeGridThreshold, vtkHyperTreeGridAlgorithm);
  void PrintSelf(ostream& os, vtkIndent indent) override;

  ///@{
  /**
   * Set/Get True, sets the MemoryStrategy to MaskInput ; False, does nothing now prefer
   * SetMemoryStrategy.
   *
   * Default is technically true
   */
  VTK_DEPRECATED_IN_9_3_0("JustCreateNewMask is deprecated in favor of MemoryStrategy")
  virtual void SetJustCreateNewMask(bool) {}
  VTK_DEPRECATED_IN_9_3_0("JustCreateNewMask is deprecated in favor of MemoryStrategy")
  virtual bool GetJustCreateNewMask() { return (this->GetMemoryStrategy() == MaskInput); }
  ///@}

  ///@{
  /**
   * Set/Get minimum scalar value of threshold
   */
  vtkSetMacro(LowerThreshold, double);
  vtkGetMacro(LowerThreshold, double);
  ///@}

  ///@{
  /**
   * Set/Get maximum scalar value of threshold
   */
  vtkSetMacro(UpperThreshold, double);
  vtkGetMacro(UpperThreshold, double);
  ///@}

  /**
   * Convenience method to set both threshold values at once
   */
  void ThresholdBetween(double, double);

  ///@{
  /**
   * Enum for defining the strategy to take in allocating the memory used by the output
   *
   * - MaskInput: shallow copy the input and generate a new mask based on the threshold
   * - CopyStructureAndIndexArrays: generate a new HTG from the minimal set of cells necessary to
   * describe the thresholded result and use `vtkIndexedArray`s to index the cell data on the input
   * - DeepThreshold: generate a new HTG from the threshold of the input HTG
   */
  enum MemoryStrategyChoice
  {
    MaskInput = 0,
    CopyStructureAndIndexArrays = 1,
    DeepThreshold = 2
  };
  /**
   * Setter and Getter for the memory strategy
   *
   * Default is MaskInput
   */
  vtkGetMacro(MemoryStrategy, int);
  vtkSetClampMacro(MemoryStrategy, int, MaskInput, DeepThreshold);
  ///@}

protected:
  vtkHyperTreeGridThreshold();
  ~vtkHyperTreeGridThreshold() override;

  /**
   * For this algorithm the output is a vtkHyperTreeGrid instance
   */
  int FillOutputPortInformation(int, vtkInformation*) override;

  /**
   * Main routine to extract cells based on thresholded value
   */
  int ProcessTrees(vtkHyperTreeGrid*, vtkDataObject*) override;

  /**
   * Recursively descend into tree down to leaves
   */
  bool RecursivelyProcessTree(
    vtkHyperTreeGridNonOrientedCursor*, vtkHyperTreeGridNonOrientedCursor*);
  bool RecursivelyProcessTreeWithCreateNewMask(vtkHyperTreeGridNonOrientedCursor*);

  /**
   * LowerThreshold scalar value to be accepted
   */
  double LowerThreshold;

  /**
   * UpperThreshold scalar value to be accepted
   */
  double UpperThreshold;

  /**
   * Input material mask
   */
  vtkBitArray* InMask;

  /**
   * Output material mask constructed by this filter
   */
  vtkBitArray* OutMask;

  /**
   * Keep track of current index in output hyper tree grid
   */
  vtkIdType CurrentId;

  /**
   * Keep track of selected input scalars
   */
  vtkDataArray* InScalars;

  /**
   * With or without copy (deprecated in favor of MemoryStrategy)
   */
  bool JustCreateNewMask;

private:
  vtkHyperTreeGridThreshold(const vtkHyperTreeGridThreshold&) = delete;
  void operator=(const vtkHyperTreeGridThreshold&) = delete;

  /**
   * The current memory strategy to use
   */
  int MemoryStrategy = MaskInput;

  struct Internals;
  std::unique_ptr<Internals> Internal;
};

VTK_ABI_NAMESPACE_END
#endif /* vtkHyperTreeGridThreshold */
