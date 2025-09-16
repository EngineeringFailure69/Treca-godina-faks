using System;
using Microsoft.EntityFrameworkCore.Migrations;

#nullable disable

namespace WebTemplate.Migrations
{
    /// <inheritdoc />
    public partial class V1 : Migration
    {
        /// <inheritdoc />
        protected override void Up(MigrationBuilder migrationBuilder)
        {
            migrationBuilder.CreateTable(
                name: "Aerodromi",
                columns: table => new
                {
                    ID = table.Column<int>(type: "int", nullable: false)
                        .Annotation("SqlServer:Identity", "1, 1"),
                    Naziv = table.Column<string>(type: "nvarchar(max)", nullable: false),
                    Kod = table.Column<string>(type: "nvarchar(3)", maxLength: 3, nullable: false),
                    Latitude = table.Column<double>(type: "float", nullable: false),
                    Longitude = table.Column<double>(type: "float", nullable: false),
                    KapacitetLetelica = table.Column<long>(type: "bigint", nullable: false),
                    KapacitetPutnika = table.Column<long>(type: "bigint", nullable: false)
                },
                constraints: table =>
                {
                    table.PrimaryKey("PK_Aerodromi", x => x.ID);
                });

            migrationBuilder.CreateTable(
                name: "Letelice",
                columns: table => new
                {
                    ID = table.Column<int>(type: "int", nullable: false)
                        .Annotation("SqlServer:Identity", "1, 1"),
                    Naziv = table.Column<string>(type: "nvarchar(max)", nullable: false),
                    KapacitetPutnika = table.Column<long>(type: "bigint", nullable: false),
                    BrojOsobaPosade = table.Column<long>(type: "bigint", nullable: false),
                    BrojMotora = table.Column<string>(type: "nvarchar(max)", nullable: false)
                },
                constraints: table =>
                {
                    table.PrimaryKey("PK_Letelice", x => x.ID);
                });

            migrationBuilder.CreateTable(
                name: "Letovi",
                columns: table => new
                {
                    ID = table.Column<int>(type: "int", nullable: false)
                        .Annotation("SqlServer:Identity", "1, 1"),
                    VremePoletanja = table.Column<DateTime>(type: "datetime2", nullable: false),
                    VremeSletanja = table.Column<DateTime>(type: "datetime2", nullable: false),
                    AerodromPoletanjaID = table.Column<int>(type: "int", nullable: true),
                    AerodromSletanjaID = table.Column<int>(type: "int", nullable: true),
                    LetelicaID = table.Column<int>(type: "int", nullable: true)
                },
                constraints: table =>
                {
                    table.PrimaryKey("PK_Letovi", x => x.ID);
                    table.ForeignKey(
                        name: "FK_Letovi_Aerodromi_AerodromPoletanjaID",
                        column: x => x.AerodromPoletanjaID,
                        principalTable: "Aerodromi",
                        principalColumn: "ID");
                    table.ForeignKey(
                        name: "FK_Letovi_Aerodromi_AerodromSletanjaID",
                        column: x => x.AerodromSletanjaID,
                        principalTable: "Aerodromi",
                        principalColumn: "ID");
                    table.ForeignKey(
                        name: "FK_Letovi_Letelice_LetelicaID",
                        column: x => x.LetelicaID,
                        principalTable: "Letelice",
                        principalColumn: "ID");
                });

            migrationBuilder.CreateIndex(
                name: "IX_Letovi_AerodromPoletanjaID",
                table: "Letovi",
                column: "AerodromPoletanjaID");

            migrationBuilder.CreateIndex(
                name: "IX_Letovi_AerodromSletanjaID",
                table: "Letovi",
                column: "AerodromSletanjaID");

            migrationBuilder.CreateIndex(
                name: "IX_Letovi_LetelicaID",
                table: "Letovi",
                column: "LetelicaID");
        }

        /// <inheritdoc />
        protected override void Down(MigrationBuilder migrationBuilder)
        {
            migrationBuilder.DropTable(
                name: "Letovi");

            migrationBuilder.DropTable(
                name: "Aerodromi");

            migrationBuilder.DropTable(
                name: "Letelice");
        }
    }
}
