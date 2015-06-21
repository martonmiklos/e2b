// This file is generated by kxml_compiler from eagle2.xsd.
// All changes you do to this file will be lost.

#include "eagle2.h"

#include <QtDebug>
#include <QFile>
#include <QDomDocument>
#include <QtCore/QtDebug>
#include <QtCore/QFile>

void Via::setX( double v )
{
  mX = v;
}

double Via::x() const
{
  return mX;
}

void Via::setY( double v )
{
  mY = v;
}

double Via::y() const
{
  return mY;
}

void Via::setExtent( const QString &v )
{
  mExtent = v;
}

QString Via::extent() const
{
  return mExtent;
}

void Via::setDrill( double v )
{
  mDrill = v;
}

double Via::drill() const
{
  return mDrill;
}

void Via::setDiameter( double v )
{
  mDiameter = v;
}

double Via::diameter() const
{
  return mDiameter;
}

void Via::setShape( const ShapeEnum &v )
{
  mShape = v;
}

Via::ShapeEnum Via::shape() const
{
  return mShape;
}

Via::ShapeEnum Via::shapeEnumFromString( const QString & v, bool *ok )
{
  if (ok) *ok = true;

  if ( v == "square" ) {
    return Shape_square;
  } else if ( v == "round-asdas" ) {
    return Shape_round_asdas;
  } else if ( v == "octagon" ) {
    return Shape_octagon;
  } else {
    if (ok) *ok = false;
    return Shape_Invalid;
  }

  return Shape_Invalid;
}

QString Via::shapeEnumToString( const ShapeEnum & v )
{
  switch( v ) {
    case Shape_square: return "square";
    case Shape_round_asdas: return "round-asdas";
    case Shape_octagon: return "octagon";
  }

}

Via Via::parseElement( const QDomElement &element, bool *ok )
{
  if ( element.tagName() != "via" ) {
    qCritical() << "Expected 'via', got '" << element.tagName() << "'.";
    if ( ok ) *ok = false;
    return Via();
  }

  Via result = Via();

  result.setX( element.attribute( "x" ).toDouble() );
  result.setY( element.attribute( "y" ).toDouble() );
  result.setExtent( element.attribute( "extent" ) );
  result.setDrill( element.attribute( "drill" ).toDouble() );
  result.setDiameter( element.attribute( "diameter" ).toDouble() );
  ShapeEnum shape = shapeEnumFromString( element.attribute( "shape" ), ok  );
  if (ok && *ok == false) {
    qCritical() << "Invalid string: \"" << element.attribute( "shape" ) << "\" in the \"shape\" element";
    return Via();
  } else {
    result.setShape( shape );
  }

  if ( ok ) *ok = true;
  return result;
}

void Via::writeElement( QXmlStreamWriter &xml ) const
{
  xml.writeEmptyElement( "via" );
}

Via Via::parseFile( const QString &filename, bool *ok )
{
  QFile file( filename );
  if ( !file.open( QIODevice::ReadOnly ) ) {
    qCritical() << "Unable to open file '" << filename << "'";
    if ( ok ) *ok = false;
    return Via();
  }

  QString errorMsg;
  int errorLine, errorCol;
  QDomDocument doc;
  if ( !doc.setContent( &file, false, &errorMsg, &errorLine, &errorCol ) ) {
    qCritical() << errorMsg << " at " << errorLine << "," << errorCol;
    if ( ok ) *ok = false;
    return Via();
  }

  bool documentOk;
  Via c = parseElement( doc.documentElement(), &documentOk );
  if ( ok ) {
    *ok = documentOk;
  }
  return c;
}

Via Via::parseString( const QString &xml, bool *ok )
{
  QString errorMsg;
  int errorLine, errorCol;
  QDomDocument doc;
  if ( !doc.setContent( xml, false, &errorMsg, &errorLine, &errorCol ) ) {
    qCritical() << errorMsg << " at " << errorLine << "," << errorCol;
    if ( ok ) *ok = false;
    return Via();
  }

  bool documentOk;
  Via c = parseElement( doc.documentElement(), &documentOk );
  if ( ok ) {
    *ok = documentOk;
  }
  return c;
}

bool Via::writeFile( const QString &filename ) const
{
  QFile file( filename );
  if ( !file.open( QIODevice::WriteOnly ) ) {
    qCritical() << "Unable to open file '" << filename << "'";
    return false;
  }

  QXmlStreamWriter xml( &file );
  xml.setAutoFormatting( true );
  xml.setAutoFormattingIndent( 2 );
  xml.writeStartDocument( "1.0" );
  writeElement( xml );
  xml.writeEndDocument();
  file.close();

  return true;
}

