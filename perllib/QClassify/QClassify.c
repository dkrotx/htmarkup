/*
 * This file was generated automatically by ExtUtils::ParseXS version 2.2210 from the
 * contents of QClassify.xs. Do not edit this file, edit QClassify.xs instead.
 *
 *	ANY CHANGES MADE HERE WILL BE LOST! 
 *
 */

#line 1 "QClassify.xs"
#ifdef __cplusplus
extern "C" {
#endif

#include "EXTERN.h"
#include "perl.h"
#include "XSUB.h"

#ifdef __cplusplus
}
#endif

#include "ppport.h"

// undef conflicting perl macroses
#ifdef Copy
#  undef Copy
#endif

#ifdef do_close
#   undef do_close
#endif

#ifdef do_open
#   undef do_open
#endif

#include <stdio.h>
#include <string>
#include "config/config.hpp"
#include "lem_interface/lem_interface.hpp"
#include "qclassify/qclassify.hpp"
#include "qclassify/qclassify_impl.hpp"
#include "qclassify/htmlmark.hpp"
using namespace gogo;


//
// Class wrapper around PhraseCollectionLoader
//
class QClassifyAgent 
{
  const PhraseSearcher *m_psrch;
  PhraseCollectionLoader m_ldr;
  static lemInterface *m_pLem;
  static int lem_nrefs;
  XmlConfig m_cfg;
  std::string m_req;
  QCHtmlMarker m_marker;

  int  m_error;

public:
  enum { ESTATUS_OK = 0, ESTATUS_CONFERROR, ESTATUS_INITERROR, ESTATUS_INDEXERROR, ESTATUS_LOADERROR };

  PhraseSearcher::res_t m_clsRes;
  int init() 
  {
    m_psrch = 0;
    try {
        if (!m_pLem) {
            assert(!lem_nrefs);
            m_pLem = new lemInterface();
        }
        lem_nrefs++;
    } catch(...) {
        return (m_error = ESTATUS_INITERROR);
    }

    return (m_error = ESTATUS_OK);
  }
  
  int loadConfig(const char *path) 
  {
    m_psrch = NULL;
    try {
        if (!m_cfg.Load(path)) 
            return (m_error = ESTATUS_CONFERROR);
    } catch(...) {
        return (m_error = ESTATUS_CONFERROR);
    }
    return ESTATUS_OK;
  }
  
  QClassifyAgent() { init(); }

  QClassifyAgent(const char *s) {
    init();
    m_error = loadConfig(s);
  }


  ~QClassifyAgent() {
    if (--lem_nrefs == 0) {
        delete m_pLem;
        m_pLem = NULL;
    }
  }

  int index2file() {
   try {
       PhraseCollectionIndexer idx(m_pLem); 
       idx.indexByConfig(&m_cfg);
       idx.save();
   } catch(...) {
        return (m_error = ESTATUS_INDEXERROR);
   }
   return ESTATUS_OK;
  }

  PhraseSearcher::res_t& searchPhrase(const char *s) {
    if (!m_psrch) 
      prepareSearch();

    m_req.assign(s);
    m_psrch->searchPhrase(m_req, m_clsRes);
    return m_clsRes;
  }

  int initMarkup() {
    int ret = prepareSearch();
    if (ret != ESTATUS_OK)
        return ret;

    m_marker.setPhraseSearcher(m_psrch);
    m_marker.loadSettings(&m_cfg);
    return ESTATUS_OK;
  }
  
  unsigned callMarkup(const std::string &s, std::string &out, 
                      const QCHtmlMarker::MarkupSettings &st) 
  {
    return m_marker.markup(s, out, st);
  }

  
  void getMarkupSettings(QCHtmlMarker::MarkupSettings *pst) {
    *pst = m_marker.getConfigSettings();
  }

  void getIndexFileName(std::string &s) {
    m_cfg.GetStr("QueryQualifier", "IndexFile", s, "phrases.idx");
  }

  int error() const { return m_error; }
  void clearerr() { m_error = ESTATUS_OK; }

private:

  int prepareSearch() {
    m_ldr.setLemmatizer(m_pLem);
    if (!m_ldr.loadByConfig(&m_cfg)) {
        return (m_error = ESTATUS_LOADERROR);
    }
        
    m_psrch = m_ldr.getSearcher();
    return ESTATUS_OK;
  }
};

lemInterface *QClassifyAgent::m_pLem = NULL;
int QClassifyAgent::lem_nrefs = 0;


#line 175 "QClassify.c"
#ifndef PERL_UNUSED_VAR
#  define PERL_UNUSED_VAR(var) if (0) var = var
#endif

#ifndef PERL_ARGS_ASSERT_CROAK_XS_USAGE
#define PERL_ARGS_ASSERT_CROAK_XS_USAGE assert(cv); assert(params)

/* prototype to pass -Wmissing-prototypes */
STATIC void
S_croak_xs_usage(pTHX_ const CV *const cv, const char *const params);

STATIC void
S_croak_xs_usage(pTHX_ const CV *const cv, const char *const params)
{
    const GV *const gv = CvGV(cv);

    PERL_ARGS_ASSERT_CROAK_XS_USAGE;

    if (gv) {
        const char *const gvname = GvNAME(gv);
        const HV *const stash = GvSTASH(gv);
        const char *const hvname = stash ? HvNAME(stash) : NULL;

        if (hvname)
            Perl_croak(aTHX_ "Usage: %s::%s(%s)", hvname, gvname, params);
        else
            Perl_croak(aTHX_ "Usage: %s(%s)", gvname, params);
    } else {
        /* Pants. I don't think that it should be possible to get here. */
        Perl_croak(aTHX_ "Usage: CODE(0x%"UVxf")(%s)", PTR2UV(cv), params);
    }
}
#undef  PERL_ARGS_ASSERT_CROAK_XS_USAGE

#ifdef PERL_IMPLICIT_CONTEXT
#define croak_xs_usage(a,b)	S_croak_xs_usage(aTHX_ a,b)
#else
#define croak_xs_usage		S_croak_xs_usage
#endif

#endif

/* NOTE: the prototype of newXSproto() is different in versions of perls,
 * so we define a portable version of newXSproto()
 */
#ifdef newXS_flags
#define newXSproto_portable(name, c_impl, file, proto) newXS_flags(name, c_impl, file, proto, 0)
#else
#define newXSproto_portable(name, c_impl, file, proto) (PL_Sv=(SV*)newXS(name, c_impl, file), sv_setpv(PL_Sv, proto), (CV*)PL_Sv)
#endif /* !defined(newXS_flags) */

#line 227 "QClassify.c"

XS(XS_QClassify_new); /* prototype to pass -Wmissing-prototypes */
XS(XS_QClassify_new)
{
#ifdef dVAR
    dVAR; dXSARGS;
#else
    dXSARGS;
#endif
    if (items != 2)
       croak_xs_usage(cv,  "CLASS, s");
    {
	char *	CLASS = (char *)SvPV_nolen(ST(0));
	QClassifyAgent *	RETVAL;
	const char *	s = (const char *)SvPV_nolen(ST(1));

	RETVAL = new QClassifyAgent(s);
	ST(0) = sv_newmortal();
	sv_setref_pv( ST(0), CLASS, (void*)RETVAL );

    }
    XSRETURN(1);
}


XS(XS_QClassify_DESTROY); /* prototype to pass -Wmissing-prototypes */
XS(XS_QClassify_DESTROY)
{
#ifdef dVAR
    dVAR; dXSARGS;
#else
    dXSARGS;
#endif
    if (items != 1)
       croak_xs_usage(cv,  "THIS");
    {
	QClassifyAgent *	THIS;

	if( sv_isobject(ST(0)) && (SvTYPE(SvRV(ST(0))) == SVt_PVMG) )
		THIS = (QClassifyAgent *)SvIV((SV*)SvRV( ST(0) ));
	else{
		warn( "QClassify::DESTROY() -- THIS is not a blessed SV reference" );
		XSRETURN_UNDEF;
	};

	delete THIS;
    }
    XSRETURN_EMPTY;
}


XS(XS_QClassify_initMarkup); /* prototype to pass -Wmissing-prototypes */
XS(XS_QClassify_initMarkup)
{
#ifdef dVAR
    dVAR; dXSARGS;
#else
    dXSARGS;
#endif
    if (items != 1)
       croak_xs_usage(cv,  "THIS");
    {
	QClassifyAgent *	THIS;

	if( sv_isobject(ST(0)) && (SvTYPE(SvRV(ST(0))) == SVt_PVMG) )
		THIS = (QClassifyAgent *)SvIV((SV*)SvRV( ST(0) ));
	else{
		warn( "QClassify::initMarkup() -- THIS is not a blessed SV reference" );
		XSRETURN_UNDEF;
	};
#line 177 "QClassify.xs"
    if (!THIS->initMarkup()) {
        XSRETURN_YES;
    }
    XSRETURN_NO;
#line 303 "QClassify.c"
    }
    XSRETURN_EMPTY;
}


XS(XS_QClassify_reinitMarkup); /* prototype to pass -Wmissing-prototypes */
XS(XS_QClassify_reinitMarkup)
{
#ifdef dVAR
    dVAR; dXSARGS;
#else
    dXSARGS;
#endif
    if (items != 1)
       croak_xs_usage(cv,  "THIS");
    {
	QClassifyAgent *	THIS;

	if( sv_isobject(ST(0)) && (SvTYPE(SvRV(ST(0))) == SVt_PVMG) )
		THIS = (QClassifyAgent *)SvIV((SV*)SvRV( ST(0) ));
	else{
		warn( "QClassify::reinitMarkup() -- THIS is not a blessed SV reference" );
		XSRETURN_UNDEF;
	};
#line 185 "QClassify.xs"
    if (!THIS->initMarkup()) {
        XSRETURN_YES;
    }
    XSRETURN_NO;
#line 333 "QClassify.c"
    }
    XSRETURN_EMPTY;
}


XS(XS_QClassify_version); /* prototype to pass -Wmissing-prototypes */
XS(XS_QClassify_version)
{
#ifdef dVAR
    dVAR; dXSARGS;
#else
    dXSARGS;
#endif
    if (items != 1)
       croak_xs_usage(cv,  "THIS");
    {
	QClassifyAgent *	THIS;
	unsigned	RETVAL;
	dXSTARG;

	if( sv_isobject(ST(0)) && (SvTYPE(SvRV(ST(0))) == SVt_PVMG) )
		THIS = (QClassifyAgent *)SvIV((SV*)SvRV( ST(0) ));
	else{
		warn( "QClassify::version() -- THIS is not a blessed SV reference" );
		XSRETURN_UNDEF;
	};
#line 193 "QClassify.xs"
    RETVAL = qcls_impl::QCLASSIFY_INDEX_VERSION;
#line 362 "QClassify.c"
	XSprePUSH; PUSHu((UV)RETVAL);
    }
    XSRETURN(1);
}


XS(XS_QClassify_clearerr); /* prototype to pass -Wmissing-prototypes */
XS(XS_QClassify_clearerr)
{
#ifdef dVAR
    dVAR; dXSARGS;
#else
    dXSARGS;
#endif
    if (items != 1)
       croak_xs_usage(cv,  "THIS");
    {
	QClassifyAgent *	THIS;

	if( sv_isobject(ST(0)) && (SvTYPE(SvRV(ST(0))) == SVt_PVMG) )
		THIS = (QClassifyAgent *)SvIV((SV*)SvRV( ST(0) ));
	else{
		warn( "QClassify::clearerr() -- THIS is not a blessed SV reference" );
		XSRETURN_UNDEF;
	};
#line 201 "QClassify.xs"
    THIS->clearerr();
#line 390 "QClassify.c"
    }
    XSRETURN_EMPTY;
}


XS(XS_QClassify_error); /* prototype to pass -Wmissing-prototypes */
XS(XS_QClassify_error)
{
#ifdef dVAR
    dVAR; dXSARGS;
#else
    dXSARGS;
#endif
    if (items != 1)
       croak_xs_usage(cv,  "THIS");
    {
	QClassifyAgent *	THIS;
	int	RETVAL;
	dXSTARG;

	if( sv_isobject(ST(0)) && (SvTYPE(SvRV(ST(0))) == SVt_PVMG) )
		THIS = (QClassifyAgent *)SvIV((SV*)SvRV( ST(0) ));
	else{
		warn( "QClassify::error() -- THIS is not a blessed SV reference" );
		XSRETURN_UNDEF;
	};
#line 206 "QClassify.xs"
    RETVAL = THIS->error();
#line 419 "QClassify.c"
	XSprePUSH; PUSHi((IV)RETVAL);
    }
    XSRETURN(1);
}


XS(XS_QClassify_loadConfig); /* prototype to pass -Wmissing-prototypes */
XS(XS_QClassify_loadConfig)
{
#ifdef dVAR
    dVAR; dXSARGS;
#else
    dXSARGS;
#endif
    if (items != 2)
       croak_xs_usage(cv,  "THIS, path");
    {
	QClassifyAgent *	THIS;
	const char *	path = (const char *)SvPV_nolen(ST(1));

	if( sv_isobject(ST(0)) && (SvTYPE(SvRV(ST(0))) == SVt_PVMG) )
		THIS = (QClassifyAgent *)SvIV((SV*)SvRV( ST(0) ));
	else{
		warn( "QClassify::loadConfig() -- THIS is not a blessed SV reference" );
		XSRETURN_UNDEF;
	};
#line 213 "QClassify.xs"
    if (!THIS->loadConfig(path)) {
        XSRETURN_YES;
    }
    XSRETURN_NO;
#line 451 "QClassify.c"
    }
    XSRETURN_EMPTY;
}


XS(XS_QClassify_index2file); /* prototype to pass -Wmissing-prototypes */
XS(XS_QClassify_index2file)
{
#ifdef dVAR
    dVAR; dXSARGS;
#else
    dXSARGS;
#endif
    if (items != 1)
       croak_xs_usage(cv,  "THIS");
    {
	QClassifyAgent *	THIS;

	if( sv_isobject(ST(0)) && (SvTYPE(SvRV(ST(0))) == SVt_PVMG) )
		THIS = (QClassifyAgent *)SvIV((SV*)SvRV( ST(0) ));
	else{
		warn( "QClassify::index2file() -- THIS is not a blessed SV reference" );
		XSRETURN_UNDEF;
	};
#line 221 "QClassify.xs"
    if (!THIS->index2file()) {
        XSRETURN_YES;
    }
    XSRETURN_NO;
#line 481 "QClassify.c"
    }
    XSRETURN_EMPTY;
}


XS(XS_QClassify_getIndexFileName); /* prototype to pass -Wmissing-prototypes */
XS(XS_QClassify_getIndexFileName)
{
#ifdef dVAR
    dVAR; dXSARGS;
#else
    dXSARGS;
#endif
    if (items != 1)
       croak_xs_usage(cv,  "THIS");
    PERL_UNUSED_VAR(ax); /* -Wall */
    SP -= items;
    {
	QClassifyAgent *	THIS;

	if( sv_isobject(ST(0)) && (SvTYPE(SvRV(ST(0))) == SVt_PVMG) )
		THIS = (QClassifyAgent *)SvIV((SV*)SvRV( ST(0) ));
	else{
		warn( "QClassify::getIndexFileName() -- THIS is not a blessed SV reference" );
		XSRETURN_UNDEF;
	};
#line 229 "QClassify.xs"
    std::string s;
    THIS->getIndexFileName(s);
    XPUSHs( sv_2mortal(newSVpv(s.c_str(), 0)) );
#line 512 "QClassify.c"
	PUTBACK;
	return;
    }
}


XS(XS_QClassify_classifyPhrase); /* prototype to pass -Wmissing-prototypes */
XS(XS_QClassify_classifyPhrase)
{
#ifdef dVAR
    dVAR; dXSARGS;
#else
    dXSARGS;
#endif
    if (items != 2)
       croak_xs_usage(cv,  "THIS, phrase");
    PERL_UNUSED_VAR(ax); /* -Wall */
    SP -= items;
    {
	QClassifyAgent *	THIS;
	const char *	phrase = (const char *)SvPV_nolen(ST(1));

	if( sv_isobject(ST(0)) && (SvTYPE(SvRV(ST(0))) == SVt_PVMG) )
		THIS = (QClassifyAgent *)SvIV((SV*)SvRV( ST(0) ));
	else{
		warn( "QClassify::classifyPhrase() -- THIS is not a blessed SV reference" );
		XSRETURN_UNDEF;
	};
#line 236 "QClassify.xs"
  PhraseSearcher::res_t &r = THIS->searchPhrase(phrase);
  if (r.size() != 0) 
  {
    EXTEND(SP, r.size() * 2);
    for(PhraseSearcher::res_t::const_iterator it = r.begin(); it != r.end(); it++) {
      XPUSHs( sv_2mortal(newSVpv(it->first.data(), it->first.length())) ); // class
      mXPUSHu(it->second); // rating (unsigned)
    }
  }
#line 551 "QClassify.c"
	PUTBACK;
	return;
    }
}


XS(XS_QClassify_markup); /* prototype to pass -Wmissing-prototypes */
XS(XS_QClassify_markup)
{
#ifdef dVAR
    dVAR; dXSARGS;
#else
    dXSARGS;
#endif
    if (items != 2)
       croak_xs_usage(cv,  "THIS, s");
    PERL_UNUSED_VAR(ax); /* -Wall */
    SP -= items;
    {
	QClassifyAgent *	THIS;
	const char *	s = (const char *)SvPV_nolen(ST(1));

	if( sv_isobject(ST(0)) && (SvTYPE(SvRV(ST(0))) == SVt_PVMG) )
		THIS = (QClassifyAgent *)SvIV((SV*)SvRV( ST(0) ));
	else{
		warn( "QClassify::markup() -- THIS is not a blessed SV reference" );
		XSRETURN_UNDEF;
	};
#line 249 "QClassify.xs"
  //HV *settings; /* settings hash */
  //SV **pval;
#line 583 "QClassify.c"
#line 252 "QClassify.xs"
  QCHtmlMarker::MarkupSettings st;
  std::string out;
  unsigned n;

  THIS->getMarkupSettings(&st);

  // initialize settings from passed hash
  /*settings = (HV *)SvRV(hashref);
  pval     = hv_fetch(settings,"uniq",4,0);
  if (pval)
    st.bUniq = (bool)SvIV(*pval);*/

  n = THIS->callMarkup((std::string)s, out, st);

  // now push returning data to stack
  XPUSHs(sv_2mortal(newSViv(n)));
  XPUSHs(sv_2mortal( newSVpv(out.data(), out.length() )));
#line 602 "QClassify.c"
	PUTBACK;
	return;
    }
}

#ifdef __cplusplus
extern "C"
#endif
XS(boot_QClassify); /* prototype to pass -Wmissing-prototypes */
XS(boot_QClassify)
{
#ifdef dVAR
    dVAR; dXSARGS;
#else
    dXSARGS;
#endif
#if (PERL_REVISION == 5 && PERL_VERSION < 9)
    char* file = __FILE__;
#else
    const char* file = __FILE__;
#endif

    PERL_UNUSED_VAR(cv); /* -W */
    PERL_UNUSED_VAR(items); /* -W */
#ifdef XS_APIVERSION_BOOTCHECK
    XS_APIVERSION_BOOTCHECK;
#endif
    XS_VERSION_BOOTCHECK ;

        newXS("QClassify::new", XS_QClassify_new, file);
        newXS("QClassify::DESTROY", XS_QClassify_DESTROY, file);
        newXS("QClassify::initMarkup", XS_QClassify_initMarkup, file);
        newXS("QClassify::reinitMarkup", XS_QClassify_reinitMarkup, file);
        newXS("QClassify::version", XS_QClassify_version, file);
        newXS("QClassify::clearerr", XS_QClassify_clearerr, file);
        newXS("QClassify::error", XS_QClassify_error, file);
        newXS("QClassify::loadConfig", XS_QClassify_loadConfig, file);
        newXS("QClassify::index2file", XS_QClassify_index2file, file);
        newXS("QClassify::getIndexFileName", XS_QClassify_getIndexFileName, file);
        newXS("QClassify::classifyPhrase", XS_QClassify_classifyPhrase, file);
        newXS("QClassify::markup", XS_QClassify_markup, file);
#if (PERL_REVISION == 5 && PERL_VERSION >= 9)
  if (PL_unitcheckav)
       call_list(PL_scopestack_ix, PL_unitcheckav);
#endif
    XSRETURN_YES;
}
