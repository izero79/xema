#include <QFile>
#include <QTextStream>
#include <QDebug>
#include "modeldataloader.h"
#include "birdmodel.h"
#include "bird.h"
#include "locationmodel.h"
#include "location.h"
#include "personmodel.h"
#include "person.h"

ModelDataLoader::ModelDataLoader(QObject *parent) :
    QObject(parent)
{
}

void ModelDataLoader::loadBirdData( BirdModel *model )
{
    QFile tiedosto( ":lajilista.csv");
    tiedosto.open(QFile::ReadOnly);
    QTextStream striimi(&tiedosto);
    if( striimi.atEnd() == false )
    {
        striimi.readLine();
    }
    while( striimi.atEnd() == false )
    {
        QString birdLine;
        birdLine = striimi.readLine();
        Bird bird( birdLine.section( ';', 0, 0 ).toInt(), birdLine.section( ';', 1, 1 ), birdLine.section( ';', 2, 2 ), birdLine.section( ';', 3, 3 ), birdLine.section( ';', 4, 4 ), birdLine.section( ';', 5, 5 ) );
        model->addItem( bird );
    }
}

void ModelDataLoader::loadLocationData(LocationModel *model)
{
    model->addItem( Location( "Kokkola", "Lintutorni" ) );
    model->addItem( Location( "Kokkola", "Parveke" ) );
}

void ModelDataLoader::loadPersonData(PersonModel *model)
{
    model->addItem( Person( "Toni", QString( "Uusimäki" ), true ) );
    model->addItem( Person( "Uni", QString( "Toosimäki" ), true ) );
}
