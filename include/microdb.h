/*
 * microdb.h - 共通定義ファイル
 */

#ifndef __INCLUDE_MICRODB_H__
#define __INCLUDE_MICRODB_H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <assert.h>
#include <limits.h>
#include <math.h>
#include "messages.h"

#define DB_PATH "/Users/Koji/.microdb/data"

/*
 * Result -- 成功/失敗を返す返り値
 */
typedef enum { OK, NG } Result;

/*
 * PAGE_SIZE -- ファイルアクセスの単位(バイト数)
 */
#define PAGE_SIZE 4096

/*
 * MAX_FILENAME -- オープンするファイルの名前の長さの上限
 */
#define MAX_FILENAME 256

/*
 * File - オープンしたファイルの情報を保持する構造体
 */
typedef struct File File;
struct File {
    int desc;                           /* ファイルディスクリプタ */
    char name[MAX_FILENAME];            /* ファイル名 */
};

/*
 * MAX_FIELD -- 1レコードに含まれるフィールド数の上限
 */
#define MAX_FIELD 40

/*
 * MAX_FIELD_NAME -- フィールド名の長さの上限(バイト数)
 */
#define MAX_FIELD_NAME 20

/*
 * MAX_STRING -- 文字列型データの長さの上限
 */
#define MAX_STRING 255

/*
 * dataType -- データベースに保存するデータの型
 */
typedef enum DataType DataType;
enum DataType {
    TYPE_UNKNOWN = 0,			/* データ型不明 */
    TYPE_INT = 1,			/* 整数型 */
    TYPE_DOUBLE = 2,             /* 倍精度浮動小数点型 */
    TYPE_VARCHAR = 3			/* 文字列型 */
};

/*
 * FieldInfo -- フィールドの情報を表現する構造体
 */
typedef struct FieldInfo FieldInfo;
struct FieldInfo {
    char name[MAX_FIELD_NAME];		/* フィールド名 */
    DataType dataType;			/* フィールドのデータ型 */
};

/*
 * TableInfo -- テーブルの情報を表現する構造体
 */
typedef struct TableInfo TableInfo;
struct TableInfo {
    int numField;				/* フィールド数 */
    FieldInfo fieldInfo[MAX_FIELD];		/* フィールド情報の配列 */
};

/*
 * FieldValue -- 1つのフィールドの値を格納する共用体
 */
typedef union FieldValue FieldValue;
union FieldValue {
    int intVal;                 /* integer型の場合の値 */
    double doubleVal;           /* double型の場合の値 */
    char stringVal[MAX_STRING]; /* varchar型の場合の値 */
};

/*
 * FieldData -- 1つのフィールドのデータを表現する構造体
 */
typedef struct FieldData FieldData;
struct FieldData {
    char name[MAX_FIELD_NAME];		/* フィールド名 */
    DataType dataType;			/* フィールドのデータ型 */
    FieldValue val;
};

/*
 * RecordData -- 1つのレコードのデータを表現する構造体
 */
typedef struct RecordData RecordData;
struct RecordData {
    int numField;			/* フィールド数 */
    FieldData fieldData[MAX_FIELD];	/* フィールド情報 */
    RecordData *next;
};

/*
 * RecordSet -- レコードの集合を表現する構造体
 */
typedef struct RecordSet RecordSet;
struct RecordSet {
    int numRecord;			/* レコード数 */
    RecordData *recordData;		/* レコードのリストへのポインタ */
};

/*
 * PageIndex -- ページ内のインデックス
 */
typedef struct Slot Slot;
struct Slot{
    int num;
    char flag;
    int offset;
    int size;
};

/*
 * OpratorType -- 比較演算子を表す列挙型
 */
typedef enum OperatorType OperatorType;
enum OperatorType {
    OPR_EQUAL,				/* = */
    OPR_NOT_EQUAL,			/* != */
    OPR_GREATER_THAN,			/* > */
    OPR_OR_GREATER_THAN,      /* >= */
    OPR_LESS_THAN,			/* < */
    OPR_OR_LESS_THAN,        /* <= */
    OPR_UNKNOWN             /*不明*/
};

/*
 * distinctFlag -- 重複除去フラグ
 */
typedef enum { NOT_DISTINCT = 0, DISTINCT = 1 } distinctFlag;


/*
 * Condition -- 検索や削除の条件式を表現する構造体
 */
typedef struct Condition Condition;
struct Condition {
    char name[MAX_FIELD_NAME];		/* フィールド名 */
    DataType dataType;			/* フィールドのデータ型 */
    OperatorType operator;		/* 比較演算子 */
    FieldValue val;              /* フィールド値].intValue */
    distinctFlag distinct;		/* 重複除去フラグ */
};

/*
 * FieldList -- select句に指定されたフィールドのリストを表現する構造体
 */
typedef struct FieldList FieldList;
struct FieldList {
    int numField;                               /* select句に指定されたフィールドの数 */
    char name[MAX_FIELD][MAX_FIELD_NAME];       /* select句に指定されたフィールド名 */
};



/*
 * file.cに定義されている関数群
 */
extern Result initializeFileModule();
extern Result finalizeFileModule();
extern Result createFile(char *);
extern Result deleteFile(char *);
extern File *openFile(char *);
extern Result closeFile(File *);
extern Result readPage(File *, int, char *);
extern Result writePage(File *, int, char *);
extern int getNumPages(char *);
extern void printBufferList();

/*
 * datadef.cに定義されている関数群
 */
extern Result initializeDataDefModule();
extern Result finalizeDataDefModule();
extern Result createTable(char *, TableInfo *);
extern Result dropTable(char *);
extern TableInfo *getTableInfo(char *);
extern void freeTableInfo(TableInfo *);

/*
 * datamanip.cに定義されている関数群
 */
extern Result initializeDataManipModule();
extern Result finalizeDataManipModule();
extern Result insertRecord(char *, RecordData *);
extern RecordSet *selectRecord(char *, FieldList *, Condition *);
extern void freeRecordSet(RecordSet *);
extern Result deleteRecord(char *, Condition *);
extern Result createDataFile(char *);
extern Result deleteDataFile(char *);

/*
 * resultprint.cに定義されている関数群
 */
extern void printTableData(char *);
extern void printRecordSet(char *, RecordSet *, FieldList *);
extern void printTableInfo(char *);
extern void printRecord(char *, RecordData *);

#endif