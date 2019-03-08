#include "Albany_TpetraThyraUtils.hpp"

#include "Albany_TpetraThyraTypes.hpp"

namespace Albany
{

struct BadThyraTpetraCast : public std::bad_cast {
  BadThyraTpetraCast (const std::string& msg)
   : m_msg (msg)
  {}

  const char * what () const noexcept { return m_msg.c_str(); }

private:
  const std::string& m_msg;
};

// ============ Tpetra->Thyra conversion routines ============ //

Teuchos::RCP<const Thyra_VectorSpace>
createThyraVectorSpace (const Teuchos::RCP<const Tpetra_Map> map)
{
  Teuchos::RCP<const Thyra_VectorSpace> vs;
  if (!map.is_null()) {
    vs = Thyra::tpetraVectorSpace<ST>(map);
  }

  return vs;
}

Teuchos::RCP<Thyra_Vector>createThyraVector (const Teuchos::RCP<Tpetra_Vector> v)
{
  Teuchos::RCP<Thyra_Vector> v_thyra = Teuchos::null;
  if (!v.is_null()) {
    v_thyra = Thyra::createVector(v);
  }

  return v_thyra;
}

Teuchos::RCP<const Thyra_Vector>createConstThyraVector (const Teuchos::RCP<const Tpetra_Vector> v)
{
  Teuchos::RCP<const Thyra_Vector> v_thyra = Teuchos::null;
  if (!v.is_null()) {
    v_thyra = Thyra::createConstVector(v);
  }

  return v_thyra;
}

Teuchos::RCP<Thyra_MultiVector>
createThyraMultiVector (const Teuchos::RCP<Tpetra_MultiVector> mv)
{
  Teuchos::RCP<Thyra_MultiVector> mv_thyra = Teuchos::null;
  if (!mv.is_null()) {
    mv_thyra = Thyra::createMultiVector(mv);
  }

  return mv_thyra;
}

Teuchos::RCP<const Thyra_MultiVector>
createConstThyraMultiVector (const Teuchos::RCP<const Tpetra_MultiVector> mv)
{
  Teuchos::RCP<const Thyra_MultiVector> mv_thyra = Teuchos::null;
  if (!mv.is_null()) {
    mv_thyra = Thyra::createConstMultiVector(mv);
  }

  return mv_thyra;
}

Teuchos::RCP<Thyra_LinearOp>
createThyraLinearOp (const Teuchos::RCP<Tpetra_Operator> op)
{
  Teuchos::RCP<Thyra_LinearOp> lop;
  if (!op.is_null()) {
    lop = Thyra::createLinearOp(op);
  }

  return lop;
}

Teuchos::RCP<const Thyra_LinearOp>
createConstThyraLinearOp (const Teuchos::RCP<const Tpetra_Operator> op)
{
  Teuchos::RCP<const Thyra_LinearOp> lop;
  if (!op.is_null()) {
    lop = Thyra::createConstLinearOp(op);
  }

  return lop;
}

// ============ Thyra->Tpetra conversion routines ============ //

Teuchos::RCP<const Tpetra_Map>
getTpetraMap (const Teuchos::RCP<const Thyra_VectorSpace> vs,
              const bool throw_on_failure)
{
  Teuchos::RCP<const Tpetra_Map> map;
  if (!vs.is_null()) {
    // There is no way to access the tpetra map in the Thyra_TpetraVectorSpace class,
    // so create a vector, grab the Tpetra_Vector, then grab the map from it
    auto v = Thyra::createMember(vs);
    map = getConstTpetraVector(v,throw_on_failure)->getMap();
  }

  return map;
}

Teuchos::RCP<Tpetra_Vector>
getTpetraVector (const Teuchos::RCP<Thyra_Vector> v,
                 const bool throw_on_failure)
{
  Teuchos::RCP<Tpetra_Vector> v_tpetra;
  if (!v.is_null()) {
    auto tmp = Teuchos::rcp_dynamic_cast<Thyra_TpetraVector>(v,throw_on_failure);
    if (!tmp.is_null()) {
      v_tpetra = tmp->getTpetraVector();
    }
  }

  return v_tpetra;
}

Teuchos::RCP<const Tpetra_Vector>
getConstTpetraVector (const Teuchos::RCP<const Thyra_Vector> v,
                      const bool throw_on_failure)

{
  Teuchos::RCP<const Tpetra_Vector> v_tpetra;
  if (!v.is_null()) {
    auto tmp = Teuchos::rcp_dynamic_cast<const Thyra_TpetraVector>(v,throw_on_failure);
    if (!tmp.is_null()) {
      v_tpetra = tmp->getConstTpetraVector();
    }
  }

  return v_tpetra;
}

Teuchos::RCP<Tpetra_MultiVector>
getTpetraMultiVector (const Teuchos::RCP<Thyra_MultiVector> mv,
                      const bool throw_on_failure)
{
  Teuchos::RCP<Tpetra_MultiVector> mv_tpetra;
  if (!mv.is_null()) {
    auto tmp = Teuchos::rcp_dynamic_cast<Thyra_TpetraMultiVector>(mv,throw_on_failure);
    if (!tmp.is_null()) {
      mv_tpetra = tmp->getTpetraMultiVector();
    }
  }

  return mv_tpetra;
}

Teuchos::RCP<const Tpetra_MultiVector>
getConstTpetraMultiVector (const Teuchos::RCP<const Thyra_MultiVector> mv,
                           const bool throw_on_failure)
{
  Teuchos::RCP<const Tpetra_MultiVector> mv_tpetra;
  if (!mv.is_null()) {
    auto tmp = Teuchos::rcp_dynamic_cast<const Thyra_TpetraMultiVector>(mv,throw_on_failure);
    if (!tmp.is_null()) {
      mv_tpetra = tmp->getConstTpetraMultiVector();
    }
  }

  return mv_tpetra;
}

Teuchos::RCP<Tpetra_Operator>
getTpetraOperator (const Teuchos::RCP<Thyra_LinearOp> lop,
                   const bool throw_on_failure)
{
  Teuchos::RCP<Tpetra_Operator> op;
  if (!lop.is_null()) {
    auto tmp = Teuchos::rcp_dynamic_cast<Thyra_TpetraLinearOp>(lop,throw_on_failure);
    if (!tmp.is_null()) {
      op = tmp->getTpetraOperator();
    }
  }

  return op;
}

Teuchos::RCP<const Tpetra_Operator>
getConstTpetraOperator (const Teuchos::RCP<const Thyra_LinearOp> lop,
                        const bool throw_on_failure)
{
  Teuchos::RCP<const Tpetra_Operator> op;
  if (!lop.is_null()) {
    auto tmp = Teuchos::rcp_dynamic_cast<const Thyra_TpetraLinearOp>(lop,throw_on_failure);
    if (!tmp.is_null()) {
      op = tmp->getConstTpetraOperator();
    }
  }

  return op;
}

Teuchos::RCP<Tpetra_CrsMatrix>
getTpetraMatrix (const Teuchos::RCP<Thyra_LinearOp> lop,
                 const bool throw_on_failure)
{
  Teuchos::RCP<Tpetra_CrsMatrix> mat;
  if (!lop.is_null()) {
    auto op = getTpetraOperator(lop,throw_on_failure);
    mat = Teuchos::rcp_dynamic_cast<Tpetra_CrsMatrix>(op,throw_on_failure);
  }

  return mat;
}

Teuchos::RCP<const Tpetra_CrsMatrix>
getConstTpetraMatrix (const Teuchos::RCP<const Thyra_LinearOp> lop,
                      const bool throw_on_failure)
{
  Teuchos::RCP<const Tpetra_CrsMatrix> mat;
  if (!lop.is_null()) {
    auto op = getConstTpetraOperator(lop,throw_on_failure);
    mat = Teuchos::rcp_dynamic_cast<const Tpetra_CrsMatrix>(op,throw_on_failure);
  }

  return mat;
}

// --- Casts taking references as inputs --- //

Teuchos::RCP<Tpetra_Vector>
getTpetraVector (Thyra_Vector& v,
                 const bool throw_on_failure)
{
  Thyra_TpetraVector* tv = dynamic_cast<Thyra_TpetraVector*>(&v);

  TEUCHOS_TEST_FOR_EXCEPTION(tv==nullptr && throw_on_failure, BadThyraTpetraCast,
                             "Error! Could not cast input Thyra_Vector to Thyra_TpetraVector.\n");

  // We allow bad cast, but once cast goes through, we *expect* pointers to be valid
  TEUCHOS_TEST_FOR_EXCEPTION(tv->getTpetraVector().is_null(), std::runtime_error,
                             "Error! The Thyra_TpetraVector object stores a null pointer.\n")  return tv->getTpetraVector();
}

Teuchos::RCP<const Tpetra_Vector>
getConstTpetraVector (const Thyra_Vector& v,
                      const bool throw_on_failure)
{
  const Thyra_TpetraVector* tv = dynamic_cast<const Thyra_TpetraVector*>(&v);

  TEUCHOS_TEST_FOR_EXCEPTION(tv==nullptr && throw_on_failure, BadThyraTpetraCast,
                             "Error! Could not cast input Thyra_Vector to Thyra_TpetraVector.\n");

  // We allow bad cast, but once cast goes through, we *expect* pointers to be valid
  TEUCHOS_TEST_FOR_EXCEPTION(tv->getConstTpetraVector().is_null(), std::runtime_error,
                             "Error! The Thyra_TpetraVector object stores a null pointer.\n")  return tv->getConstTpetraVector();
}

Teuchos::RCP<Tpetra_MultiVector>
getTpetraMultiVector (Thyra_MultiVector& mv,
                      const bool throw_on_failure)
{
  Thyra_TpetraMultiVector* tmv = dynamic_cast<Thyra_TpetraMultiVector*>(&mv);

  TEUCHOS_TEST_FOR_EXCEPTION(tmv==nullptr && throw_on_failure, BadThyraTpetraCast,
                             "Error! Could not cast input Thyra_MultiVector to Thyra_TpetraMultiVector.\n");

  // We allow bad cast, but once cast goes through, we *expect* pointers to be valid
  TEUCHOS_TEST_FOR_EXCEPTION(tmv->getTpetraMultiVector().is_null(), std::runtime_error,
                             "Error! The Thyra_TpetraMultiVector object stores a null pointer.\n")  return tmv->getTpetraMultiVector();
}

Teuchos::RCP<const Tpetra_MultiVector>
getConstTpetraMultiVector (const Thyra_MultiVector& mv,
                           const bool throw_on_failure)
{
  const Thyra_TpetraMultiVector* tmv = dynamic_cast<const Thyra_TpetraMultiVector*>(&mv);

  TEUCHOS_TEST_FOR_EXCEPTION(tmv==nullptr && throw_on_failure, BadThyraTpetraCast,
                             "Error! Could not cast input Thyra_MultiVector to Thyra_TpetraMultiVector.\n");

  // We allow bad cast, but once cast goes through, we *expect* pointers to be valid
  TEUCHOS_TEST_FOR_EXCEPTION(tmv->getConstTpetraMultiVector().is_null(), std::runtime_error,
                             "Error! The Thyra_TpetraMultiVector object stores a null pointer.\n")  return tmv->getConstTpetraMultiVector();
}

Teuchos::RCP<Tpetra_Operator>
getTpetraOperator (Thyra_LinearOp& lop,
                   const bool throw_on_failure)
{
  Thyra_TpetraLinearOp* top = dynamic_cast<Thyra_TpetraLinearOp*>(&lop);

  TEUCHOS_TEST_FOR_EXCEPTION(top==nullptr && throw_on_failure, BadThyraTpetraCast,
                             "Error! Could not cast input Thyra_LinearOp to Thyra_TpetraLinearOp.\n");

  // We allow bad cast, but once cast goes through, we *expect* pointers to be valid
  TEUCHOS_TEST_FOR_EXCEPTION(top->getTpetraOperator().is_null(), std::runtime_error,
                             "Error! The Thyra_TpetraLinearOp object stores a null pointer.\n")  return top->getTpetraOperator();
}

Teuchos::RCP<const Tpetra_Operator>
getConstTpetraOperator (const Thyra_LinearOp& lop,
                        const bool throw_on_failure)
{
  const Thyra_TpetraLinearOp* top = dynamic_cast<const Thyra_TpetraLinearOp*>(&lop);

  TEUCHOS_TEST_FOR_EXCEPTION(top==nullptr && throw_on_failure, BadThyraTpetraCast,
                             "Error! Could not cast input Thyra_LinearOp to Thyra_TpetraLinearOp.\n");

  // We allow bad cast, but once cast goes through, we *expect* pointers to be valid
  TEUCHOS_TEST_FOR_EXCEPTION(top->getConstTpetraOperator().is_null(), std::runtime_error,
                             "Error! The Thyra_TpetraLinearOp object stores a null pointer.\n")  return top->getConstTpetraOperator();
}

Teuchos::RCP<Tpetra_CrsMatrix>
getTpetraMatrix (Thyra_LinearOp& lop,
                 const bool throw_on_failure)
{
  Thyra_TpetraLinearOp* top = dynamic_cast<Thyra_TpetraLinearOp*>(&lop);

  TEUCHOS_TEST_FOR_EXCEPTION(top==nullptr && throw_on_failure, BadThyraTpetraCast,
                             "Error! Could not cast input Thyra_LinearOp to Thyra_TpetraLinearOp.\n");

  // We allow bad cast, but once cast goes through, we *expect* pointers to be valid
  TEUCHOS_TEST_FOR_EXCEPTION(top->getTpetraOperator().is_null(), std::runtime_error,
                             "Error! The Thyra_TpetraLinearOp object stores a null pointer.\n")
  auto tmat = Teuchos::rcp_dynamic_cast<Tpetra_CrsMatrix>(top->getTpetraOperator());
  // We allow bad cast, but once cast goes through, we *expect* the operator to store a crs matrix
  TEUCHOS_TEST_FOR_EXCEPTION(tmat.is_null(), std::runtime_error,
                             "Error! The Thyra_TpetraLinearOp object does not store a Tpetra_CrsMatrix.\n")  return tmat;
}

Teuchos::RCP<const Tpetra_CrsMatrix>
getConstTpetraMatrix (const Thyra_LinearOp& lop,
                      const bool throw_on_failure)
{
  const Thyra_TpetraLinearOp* top = dynamic_cast<const Thyra_TpetraLinearOp*>(&lop);

  TEUCHOS_TEST_FOR_EXCEPTION(top==nullptr && throw_on_failure, BadThyraTpetraCast,
                             "Error! Could not cast input Thyra_LinearOp to Thyra_TpetraLinearOp.\n");

  // We allow bad cast, but once cast goes through, we *expect* pointers to be valid
  TEUCHOS_TEST_FOR_EXCEPTION(top->getConstTpetraOperator().is_null(), std::runtime_error,
                             "Error! The Thyra_TpetraLinearOp object stores a null pointer.\n")
  auto tmat = Teuchos::rcp_dynamic_cast<const Tpetra_CrsMatrix>(top->getConstTpetraOperator());
  // We allow bad cast, but once cast goes through, we *expect* the operator to store a crs matrix
  TEUCHOS_TEST_FOR_EXCEPTION(tmat.is_null(), std::runtime_error,
                             "Error! The Thyra_TpetraLinearOp object does not store a Tpetra_CrsMatrix.\n")  return tmat;
}

// --- Get a Tpetra graph from a ThyraCrsGraphProxy --- //
Teuchos::RCP<Tpetra_CrsGraph>
getTpetraCrsGraph (const ThyraCrsGraphProxy& graphProxy,
                   const bool throw_on_failure) {
  Teuchos::RCP<Tpetra_CrsGraph> t_graph;

  Teuchos::RCP<bool> dummy = graphProxy.getGraph();
  // The actual graph is stored as extra data attached to the dummy RCP returned by getGraph().
  if (throw_on_failure) {
    t_graph = Teuchos::get_nonconst_extra_data<Teuchos::RCP<Tpetra_CrsGraph>>(dummy,"Tpetra_CrsGraph");
  } else {
    auto ptr = Teuchos::get_optional_nonconst_extra_data<Teuchos::RCP<Tpetra_CrsGraph>>(dummy,"Tpetra_CrsGraph");
    if (!ptr.is_null()) {
      t_graph = *ptr;
    }
  }
  return t_graph;
}

} // namespace Albany
