#include "TextManager.h"

TextManager* TextManager::Instance = nullptr;
const char* fn = "systemFonts.txt";

TextManager& TextManager::IO()
{
	if (!Instance) {
		printf("TextManager::IO::Initializing TextManager Instance\n");
		Instance = new TextManager();
		Instance->LoadFonts(fn);
	}

	return *Instance;
}

void TextManager::DestroyCurrentInstance()
{
	delete Instance;
	Instance = nullptr;
}

std::vector<std::tuple<std::string, std::string>>& TextManager::GetFonts()
{
	return TextManager::IO().fonts;
}

void TextManager::ChangeFont(const char* fontName)
{
	m_textRenderer = new TextRenderer(fontName);
}

void TextManager::ShiftFont(const int& dist)
{
	int attempIndex = this->currentFont + dist, nextIndex = -1;
	if (attempIndex >= fonts.size())
	{
		nextIndex = 0;
	}
	if (attempIndex < 0)
	{
		nextIndex = fonts.size() - 1;
	}


	if (nextIndex != -1)
		currentFont = nextIndex;
	else
		currentFont = attempIndex;

	printf("Changed font index from: %d to: %d\n", currentFont, nextIndex);

	m_textRenderer = new TextRenderer(std::get<1>(fonts[currentFont]).c_str());
	m_textRenderer->transform.position = glm::vec3(25.0f, 130.0f, 0.0f);
	m_textRenderer->transform.scale = 0.5f;
}

void TextManager::DrawText(const char* string, const glm::vec2& position, const float& size, const glm::vec4& color)
{
	m_textRenderer->DrawText(string, position.x, position.y, size, color);
}

//void TextManager::DrawText(std::string string, const glm::vec2& position, const float& size, const glm::vec4& color)
//{
//	m_textRenderer->DrawText(string, position.x, position.y, size, color);
//}

void TextManager::StartLine(const glm::vec2& screen_pos, const float& fontSize)
{
	lineStartingPosition = screen_pos;
	Lines.clear();
	lineCount = 0;
	lineScaling = fontSize;
}

void TextManager::AddLine(const char* newLine, const glm::vec4& color)
{
	Lines.push_back(std::make_tuple(newLine, color));
	lineCount++;
}

void TextManager::EndLine()
{
	DrawLines();
	lineStartingPosition = glm::vec2(0.0f);
	Lines.clear();
}

TextManager::TextManager()
	: currentFont(0)
	, lineCount(0)
	, lineStartingPosition(0.0f)
	, lineOffset(0.0f, -36.0f)
	, lineScaling(1.0f)
{
	m_textRenderer = new TextRenderer();
}
TextManager::~TextManager()
{
	delete m_textRenderer;
}

void TextManager::DrawLines()
{
	glm::vec2 position = lineStartingPosition;
	glm::vec2 offset = glm::vec2(0.0f);

	for (int i = 0; i < lineCount; i++)
	{
		position += offset;
		std::string line = std::get<0>(Lines[i]);
		DrawText(line.c_str(), position, lineScaling, std::get<1>(Lines[i]));
		offset = lineOffset * lineScaling;
	}
}

void TextManager::LoadFonts(const char* fileName)
{
	currentFont = 0;
	fonts.push_back(std::make_tuple("OxygenMono-Regular", "./assets/fonts/OxygenMono-Regular.otf"));
	fonts.push_back(std::make_tuple("Dk Hand Regular-orna", "./assets/fonts/DkHandRegular-orna.ttf"));
	fonts.push_back(std::make_tuple("joystix monospace", "./assets/fonts/joystix monospace.otf"));
	fonts.push_back(std::make_tuple("Times New roman", "C:/Windows/Fonts/times.ttf"));
	fonts.push_back(std::make_tuple("Arial", "C:/Windows/Fonts/arial.ttf"));

	fonts.push_back(std::make_tuple("AGENCYB.TTF    ", "C:/Windows/Fonts/AGENCYB.TTF"));
	fonts.push_back(std::make_tuple("AGENCYR.TTF    ", "C:/Windows/Fonts/AGENCYR.TTF"));
	fonts.push_back(std::make_tuple("ALGER.TTF      ", "C:/Windows/Fonts/ALGER.TTF"));
	fonts.push_back(std::make_tuple("ANTQUAB.TTF    ", "C:/Windows/Fonts/ANTQUAB.TTF"));
	fonts.push_back(std::make_tuple("ANTQUABI.TTF   ", "C:/Windows/Fonts/ANTQUABI.TTF"));
	fonts.push_back(std::make_tuple("ANTQUAI.TTF    ", "C:/Windows/Fonts/ANTQUAI.TTF"));
	fonts.push_back(std::make_tuple("arial.ttf      ", "C:/Windows/Fonts/arial.ttf"));
	fonts.push_back(std::make_tuple("arialbd.ttf    ", "C:/Windows/Fonts/arialbd.ttf"));
	fonts.push_back(std::make_tuple("arialbi.ttf    ", "C:/Windows/Fonts/arialbi.ttf"));
	fonts.push_back(std::make_tuple("ariali.ttf     ", "C:/Windows/Fonts/ariali.ttf"));
	fonts.push_back(std::make_tuple("ARIALN.TTF     ", "C:/Windows/Fonts/ARIALN.TTF"));
	fonts.push_back(std::make_tuple("ARIALNB.TTF    ", "C:/Windows/Fonts/ARIALNB.TTF"));
	fonts.push_back(std::make_tuple("ARIALNBI.TTF   ", "C:/Windows/Fonts/ARIALNBI.TTF"));
	fonts.push_back(std::make_tuple("ARIALNI.TTF    ", "C:/Windows/Fonts/ARIALNI.TTF"));
	fonts.push_back(std::make_tuple("ariblk.ttf     ", "C:/Windows/Fonts/ariblk.ttf"));
	fonts.push_back(std::make_tuple("ARLRDBD.TTF    ", "C:/Windows/Fonts/ARLRDBD.TTF"));
	fonts.push_back(std::make_tuple("bahnschrift.ttf", "C:/Windows/Fonts/bahnschrift.ttf"));
	fonts.push_back(std::make_tuple("BASKVILL.TTF   ", "C:/Windows/Fonts/BASKVILL.TTF"));
	fonts.push_back(std::make_tuple("BAUHS93.TTF    ", "C:/Windows/Fonts/BAUHS93.TTF"));
	fonts.push_back(std::make_tuple("BELL.TTF       ", "C:/Windows/Fonts/BELL.TTF"));
	fonts.push_back(std::make_tuple("BELLB.TTF      ", "C:/Windows/Fonts/BELLB.TTF"));
	fonts.push_back(std::make_tuple("BELLI.TTF      ", "C:/Windows/Fonts/BELLI.TTF"));
	fonts.push_back(std::make_tuple("BERNHC.TTF     ", "C:/Windows/Fonts/BERNHC.TTF"));
	fonts.push_back(std::make_tuple("BKANT.TTF      ", "C:/Windows/Fonts/BKANT.TTF"));
	fonts.push_back(std::make_tuple("BOD_B.TTF      ", "C:/Windows/Fonts/BOD_B.TTF"));
	fonts.push_back(std::make_tuple("BOD_BI.TTF     ", "C:/Windows/Fonts/BOD_BI.TTF"));
	fonts.push_back(std::make_tuple("BOD_BLAI.TTF   ", "C:/Windows/Fonts/BOD_BLAI.TTF"));
	fonts.push_back(std::make_tuple("BOD_BLAR.TTF   ", "C:/Windows/Fonts/BOD_BLAR.TTF"));
	fonts.push_back(std::make_tuple("BOD_CB.TTF     ", "C:/Windows/Fonts/BOD_CB.TTF"));
	fonts.push_back(std::make_tuple("BOD_CBI.TTF    ", "C:/Windows/Fonts/BOD_CBI.TTF"));
	fonts.push_back(std::make_tuple("BOD_CI.TTF     ", "C:/Windows/Fonts/BOD_CI.TTF"));
	fonts.push_back(std::make_tuple("BOD_CR.TTF     ", "C:/Windows/Fonts/BOD_CR.TTF"));
	fonts.push_back(std::make_tuple("BOD_I.TTF      ", "C:/Windows/Fonts/BOD_I.TTF"));
	fonts.push_back(std::make_tuple("BOD_PSTC.TTF   ", "C:/Windows/Fonts/BOD_PSTC.TTF"));
	fonts.push_back(std::make_tuple("BOD_R.TTF      ", "C:/Windows/Fonts/BOD_R.TTF"));
	fonts.push_back(std::make_tuple("BOOKOS.TTF     ", "C:/Windows/Fonts/BOOKOS.TTF"));
	fonts.push_back(std::make_tuple("BOOKOSB.TTF    ", "C:/Windows/Fonts/BOOKOSB.TTF"));
	fonts.push_back(std::make_tuple("BOOKOSBI.TTF   ", "C:/Windows/Fonts/BOOKOSBI.TTF"));
	fonts.push_back(std::make_tuple("BOOKOSI.TTF    ", "C:/Windows/Fonts/BOOKOSI.TTF"));
	fonts.push_back(std::make_tuple("BRADHITC.TTF   ", "C:/Windows/Fonts/BRADHITC.TTF"));
	fonts.push_back(std::make_tuple("BRITANIC.TTF   ", "C:/Windows/Fonts/BRITANIC.TTF"));
	fonts.push_back(std::make_tuple("BRLNSB.TTF     ", "C:/Windows/Fonts/BRLNSB.TTF"));
	fonts.push_back(std::make_tuple("BRLNSDB.TTF    ", "C:/Windows/Fonts/BRLNSDB.TTF"));
	fonts.push_back(std::make_tuple("BRLNSR.TTF     ", "C:/Windows/Fonts/BRLNSR.TTF"));
	fonts.push_back(std::make_tuple("BROADW.TTF     ", "C:/Windows/Fonts/BROADW.TTF"));
	fonts.push_back(std::make_tuple("BRUSHSCI.TTF   ", "C:/Windows/Fonts/BRUSHSCI.TTF"));
	fonts.push_back(std::make_tuple("BSSYM7.TTF     ", "C:/Windows/Fonts/BSSYM7.TTF"));
	fonts.push_back(std::make_tuple("calibri.ttf    ", "C:/Windows/Fonts/calibri.ttf"));
	fonts.push_back(std::make_tuple("calibrib.ttf   ", "C:/Windows/Fonts/calibrib.ttf"));
	fonts.push_back(std::make_tuple("calibrii.ttf   ", "C:/Windows/Fonts/calibrii.ttf"));
	fonts.push_back(std::make_tuple("calibril.ttf   ", "C:/Windows/Fonts/calibril.ttf"));
	fonts.push_back(std::make_tuple("calibrili.ttf  ", "C:/Windows/Fonts/calibrili.ttf"));
	fonts.push_back(std::make_tuple("calibriz.ttf   ", "C:/Windows/Fonts/calibriz.ttf"));
	fonts.push_back(std::make_tuple("CALIFB.TTF     ", "C:/Windows/Fonts/CALIFB.TTF"));
	fonts.push_back(std::make_tuple("CALIFI.TTF     ", "C:/Windows/Fonts/CALIFI.TTF"));
	fonts.push_back(std::make_tuple("CALIFR.TTF     ", "C:/Windows/Fonts/CALIFR.TTF"));
	fonts.push_back(std::make_tuple("CALIST.TTF     ", "C:/Windows/Fonts/CALIST.TTF"));
	fonts.push_back(std::make_tuple("CALISTB.TTF    ", "C:/Windows/Fonts/CALISTB.TTF"));
	fonts.push_back(std::make_tuple("CALISTBI.TTF   ", "C:/Windows/Fonts/CALISTBI.TTF"));
	fonts.push_back(std::make_tuple("CALISTI.TTF    ", "C:/Windows/Fonts/CALISTI.TTF"));
	fonts.push_back(std::make_tuple("cambriab.ttf   ", "C:/Windows/Fonts/cambriab.ttf"));
	fonts.push_back(std::make_tuple("cambriai.ttf   ", "C:/Windows/Fonts/cambriai.ttf"));
	fonts.push_back(std::make_tuple("cambriaz.ttf   ", "C:/Windows/Fonts/cambriaz.ttf"));
	fonts.push_back(std::make_tuple("Candara.ttf    ", "C:/Windows/Fonts/Candara.ttf"));
	fonts.push_back(std::make_tuple("Candarab.ttf   ", "C:/Windows/Fonts/Candarab.ttf"));
	fonts.push_back(std::make_tuple("Candarai.ttf   ", "C:/Windows/Fonts/Candarai.ttf"));
	fonts.push_back(std::make_tuple("Candaral.ttf   ", "C:/Windows/Fonts/Candaral.ttf"));
	fonts.push_back(std::make_tuple("Candarali.ttf  ", "C:/Windows/Fonts/Candarali.ttf"));
	fonts.push_back(std::make_tuple("Candaraz.ttf   ", "C:/Windows/Fonts/Candaraz.ttf"));

	fonts.push_back(std::make_tuple("CascadiaCode.ttf", "C:/Windows/Fonts/CascadiaCode.ttf"));
	fonts.push_back(std::make_tuple("CascadiaMono.ttf", "C:/Windows/Fonts/CascadiaMono.ttf"));
	fonts.push_back(std::make_tuple("CASTELAR.TTF    ", "C:/Windows/Fonts/CASTELAR.TTF"));
	fonts.push_back(std::make_tuple("CENSCBK.TTF     ", "C:/Windows/Fonts/CENSCBK.TTF"));
	fonts.push_back(std::make_tuple("CENTAUR.TTF     ", "C:/Windows/Fonts/CENTAUR.TTF"));
	fonts.push_back(std::make_tuple("CENTURY.TTF     ", "C:/Windows/Fonts/CENTURY.TTF"));
	fonts.push_back(std::make_tuple("CHILLER.TTF     ", "C:/Windows/Fonts/CHILLER.TTF"));
	fonts.push_back(std::make_tuple("COLONNA.TTF     ", "C:/Windows/Fonts/COLONNA.TTF"));
	fonts.push_back(std::make_tuple("comic.ttf       ", "C:/Windows/Fonts/comic.ttf"));
	fonts.push_back(std::make_tuple("comicbd.ttf     ", "C:/Windows/Fonts/comicbd.ttf"));
	fonts.push_back(std::make_tuple("comici.ttf      ", "C:/Windows/Fonts/comici.ttf"));
	fonts.push_back(std::make_tuple("comicz.ttf      ", "C:/Windows/Fonts/comicz.ttf"));
	fonts.push_back(std::make_tuple("consola.ttf     ", "C:/Windows/Fonts/consola.ttf"));
	fonts.push_back(std::make_tuple("consolab.ttf    ", "C:/Windows/Fonts/consolab.ttf"));
	fonts.push_back(std::make_tuple("consolai.ttf    ", "C:/Windows/Fonts/consolai.ttf"));
	fonts.push_back(std::make_tuple("consolaz.ttf    ", "C:/Windows/Fonts/consolaz.ttf"));
	fonts.push_back(std::make_tuple("constan.ttf     ", "C:/Windows/Fonts/constan.ttf"));
	fonts.push_back(std::make_tuple("constanb.ttf    ", "C:/Windows/Fonts/constanb.ttf"));
	fonts.push_back(std::make_tuple("constani.ttf    ", "C:/Windows/Fonts/constani.ttf"));
	fonts.push_back(std::make_tuple("constanz.ttf    ", "C:/Windows/Fonts/constanz.ttf"));
	fonts.push_back(std::make_tuple("COOPBL.TTF      ", "C:/Windows/Fonts/COOPBL.TTF"));
	fonts.push_back(std::make_tuple("COPRGTB.TTF     ", "C:/Windows/Fonts/COPRGTB.TTF"));
	fonts.push_back(std::make_tuple("COPRGTL.TTF     ", "C:/Windows/Fonts/COPRGTL.TTF"));
	fonts.push_back(std::make_tuple("corbel.ttf      ", "C:/Windows/Fonts/corbel.ttf"));
	fonts.push_back(std::make_tuple("corbelb.ttf     ", "C:/Windows/Fonts/corbelb.ttf"));
	fonts.push_back(std::make_tuple("corbeli.ttf     ", "C:/Windows/Fonts/corbeli.ttf"));
	fonts.push_back(std::make_tuple("corbell.ttf     ", "C:/Windows/Fonts/corbell.ttf"));
	fonts.push_back(std::make_tuple("corbelli.ttf    ", "C:/Windows/Fonts/corbelli.ttf"));
	fonts.push_back(std::make_tuple("corbelz.ttf     ", "C:/Windows/Fonts/corbelz.ttf"));
	fonts.push_back(std::make_tuple("cour.ttf        ", "C:/Windows/Fonts/cour.ttf"));
	fonts.push_back(std::make_tuple("courbd.ttf      ", "C:/Windows/Fonts/courbd.ttf"));
	fonts.push_back(std::make_tuple("courbi.ttf      ", "C:/Windows/Fonts/courbi.ttf"));
	fonts.push_back(std::make_tuple("couri.ttf       ", "C:/Windows/Fonts/couri.ttf"));
	fonts.push_back(std::make_tuple("CURLZ___.TTF    ", "C:/Windows/Fonts/CURLZ___.TTF"));
	fonts.push_back(std::make_tuple("DUBAI - BOLD.TTF   ", "C:/Windows/Fonts/DUBAI - BOLD.TTF"));
	fonts.push_back(std::make_tuple("DUBAI - LIGHT.TTF  ", "C:/Windows/Fonts/DUBAI - LIGHT.TTF"));
	fonts.push_back(std::make_tuple("DUBAI - MEDIUM.TTF ", "C:/Windows/Fonts/DUBAI - MEDIUM.TTF"));
	fonts.push_back(std::make_tuple("DUBAI - REGULAR.TTF", "C:/Windows/Fonts/DUBAI - REGULAR.TTF"));

	fonts.push_back(std::make_tuple("ebrima.ttf       ", "C:/Windows/Fonts/ebrima.ttf"));
	fonts.push_back(std::make_tuple("ebrimabd.ttf     ", "C:/Windows/Fonts/ebrimabd.ttf"));
	fonts.push_back(std::make_tuple("ELEPHNT.TTF      ", "C:/Windows/Fonts/ELEPHNT.TTF"));
	fonts.push_back(std::make_tuple("ELEPHNTI.TTF     ", "C:/Windows/Fonts/ELEPHNTI.TTF"));
	fonts.push_back(std::make_tuple("ENGR.TTF         ", "C:/Windows/Fonts/ENGR.TTF"));
	fonts.push_back(std::make_tuple("ERASBD.TTF       ", "C:/Windows/Fonts/ERASBD.TTF"));
	fonts.push_back(std::make_tuple("ERASDEMI.TTF     ", "C:/Windows/Fonts/ERASDEMI.TTF"));
	fonts.push_back(std::make_tuple("ERASLGHT.TTF     ", "C:/Windows/Fonts/ERASLGHT.TTF"));
	fonts.push_back(std::make_tuple("ERASMD.TTF       ", "C:/Windows/Fonts/ERASMD.TTF"));
	fonts.push_back(std::make_tuple("FELIXTI.TTF      ", "C:/Windows/Fonts/FELIXTI.TTF"));
	fonts.push_back(std::make_tuple("FORTE.TTF        ", "C:/Windows/Fonts/FORTE.TTF"));
	fonts.push_back(std::make_tuple("FRABK.TTF        ", "C:/Windows/Fonts/FRABK.TTF"));
	fonts.push_back(std::make_tuple("FRABKIT.TTF      ", "C:/Windows/Fonts/FRABKIT.TTF"));
	fonts.push_back(std::make_tuple("FRADM.TTF        ", "C:/Windows/Fonts/FRADM.TTF"));
	fonts.push_back(std::make_tuple("FRADMCN.TTF      ", "C:/Windows/Fonts/FRADMCN.TTF"));
	fonts.push_back(std::make_tuple("FRADMIT.TTF      ", "C:/Windows/Fonts/FRADMIT.TTF"));
	fonts.push_back(std::make_tuple("FRAHV.TTF        ", "C:/Windows/Fonts/FRAHV.TTF"));
	fonts.push_back(std::make_tuple("FRAHVIT.TTF      ", "C:/Windows/Fonts/FRAHVIT.TTF"));
	fonts.push_back(std::make_tuple("framd.ttf        ", "C:/Windows/Fonts/framd.ttf"));
	fonts.push_back(std::make_tuple("FRAMDCN.TTF      ", "C:/Windows/Fonts/FRAMDCN.TTF"));
	fonts.push_back(std::make_tuple("framdit.ttf      ", "C:/Windows/Fonts/framdit.ttf"));
	fonts.push_back(std::make_tuple("FREESCPT.TTF     ", "C:/Windows/Fonts/FREESCPT.TTF"));
	fonts.push_back(std::make_tuple("FRSCRIPT.TTF     ", "C:/Windows/Fonts/FRSCRIPT.TTF"));
	fonts.push_back(std::make_tuple("FTLTLT.TTF       ", "C:/Windows/Fonts/FTLTLT.TTF"));
	fonts.push_back(std::make_tuple("Gabriola.ttf     ", "C:/Windows/Fonts/Gabriola.ttf"));
	fonts.push_back(std::make_tuple("gadugi.ttf       ", "C:/Windows/Fonts/gadugi.ttf"));
	fonts.push_back(std::make_tuple("gadugib.ttf      ", "C:/Windows/Fonts/gadugib.ttf"));
	fonts.push_back(std::make_tuple("GARA.TTF         ", "C:/Windows/Fonts/GARA.TTF"));
	fonts.push_back(std::make_tuple("GARABD.TTF       ", "C:/Windows/Fonts/GARABD.TTF"));
	fonts.push_back(std::make_tuple("GARAIT.TTF       ", "C:/Windows/Fonts/GARAIT.TTF"));
	fonts.push_back(std::make_tuple("georgia.ttf      ", "C:/Windows/Fonts/georgia.ttf"));
	fonts.push_back(std::make_tuple("georgiab.ttf     ", "C:/Windows/Fonts/georgiab.ttf"));
	fonts.push_back(std::make_tuple("georgiai.ttf     ", "C:/Windows/Fonts/georgiai.ttf"));
	fonts.push_back(std::make_tuple("georgiaz.ttf     ", "C:/Windows/Fonts/georgiaz.ttf"));
	fonts.push_back(std::make_tuple("GIGI.TTF         ", "C:/Windows/Fonts/GIGI.TTF"));
	fonts.push_back(std::make_tuple("GILBI___.TTF     ", "C:/Windows/Fonts/GILBI___.TTF"));
	fonts.push_back(std::make_tuple("GILB____.TTF     ", "C:/Windows/Fonts/GILB____.TTF"));
	fonts.push_back(std::make_tuple("GILC____.TTF     ", "C:/Windows/Fonts/GILC____.TTF"));
	fonts.push_back(std::make_tuple("GILI____.TTF     ", "C:/Windows/Fonts/GILI____.TTF"));
	fonts.push_back(std::make_tuple("GILLUBCD.TTF     ", "C:/Windows/Fonts/GILLUBCD.TTF"));
	fonts.push_back(std::make_tuple("GILSANUB.TTF     ", "C:/Windows/Fonts/GILSANUB.TTF"));
	fonts.push_back(std::make_tuple("GIL_____.TTF     ", "C:/Windows/Fonts/GIL_____.TTF"));
	fonts.push_back(std::make_tuple("GLECB.TTF        ", "C:/Windows/Fonts/GLECB.TTF"));
	fonts.push_back(std::make_tuple("GLSNECB.TTF      ", "C:/Windows/Fonts/GLSNECB.TTF"));
	fonts.push_back(std::make_tuple("GOTHIC.TTF       ", "C:/Windows/Fonts/GOTHIC.TTF"));
	fonts.push_back(std::make_tuple("GOTHICB.TTF      ", "C:/Windows/Fonts/GOTHICB.TTF"));
	fonts.push_back(std::make_tuple("GOTHICBI.TTF     ", "C:/Windows/Fonts/GOTHICBI.TTF"));
	fonts.push_back(std::make_tuple("GOTHICI.TTF      ", "C:/Windows/Fonts/GOTHICI.TTF"));
	fonts.push_back(std::make_tuple("GOUDOS.TTF       ", "C:/Windows/Fonts/GOUDOS.TTF"));
	fonts.push_back(std::make_tuple("GOUDOSB.TTF      ", "C:/Windows/Fonts/GOUDOSB.TTF"));
	fonts.push_back(std::make_tuple("GOUDOSI.TTF      ", "C:/Windows/Fonts/GOUDOSI.TTF"));
	fonts.push_back(std::make_tuple("GOUDYSTO.TTF     ", "C:/Windows/Fonts/GOUDYSTO.TTF"));
	fonts.push_back(std::make_tuple("HARLOWSI.TTF     ", "C:/Windows/Fonts/HARLOWSI.TTF"));
	fonts.push_back(std::make_tuple("HARNGTON.TTF     ", "C:/Windows/Fonts/HARNGTON.TTF"));
	fonts.push_back(std::make_tuple("HATTEN.TTF       ", "C:/Windows/Fonts/HATTEN.TTF"));
	fonts.push_back(std::make_tuple("himalaya.ttf     ", "C:/Windows/Fonts/himalaya.ttf"));
	fonts.push_back(std::make_tuple("holomdl2.ttf     ", "C:/Windows/Fonts/holomdl2.ttf"));
	fonts.push_back(std::make_tuple("HTOWERT.TTF      ", "C:/Windows/Fonts/HTOWERT.TTF"));
	fonts.push_back(std::make_tuple("HTOWERTI.TTF     ", "C:/Windows/Fonts/HTOWERTI.TTF"));
	fonts.push_back(std::make_tuple("impact.ttf       ", "C:/Windows/Fonts/impact.ttf"));
	fonts.push_back(std::make_tuple("IMPRISHA.TTF     ", "C:/Windows/Fonts/IMPRISHA.TTF"));
	fonts.push_back(std::make_tuple("INFROMAN.TTF     ", "C:/Windows/Fonts/INFROMAN.TTF"));
	fonts.push_back(std::make_tuple("Inkfree.ttf      ", "C:/Windows/Fonts/Inkfree.ttf"));
	fonts.push_back(std::make_tuple("ITCBLKAD.TTF     ", "C:/Windows/Fonts/ITCBLKAD.TTF"));
	fonts.push_back(std::make_tuple("ITCEDSCR.TTF     ", "C:/Windows/Fonts/ITCEDSCR.TTF"));
	fonts.push_back(std::make_tuple("ITCKRIST.TTF     ", "C:/Windows/Fonts/ITCKRIST.TTF"));
	fonts.push_back(std::make_tuple("javatext.ttf     ", "C:/Windows/Fonts/javatext.ttf"));
	fonts.push_back(std::make_tuple("JOKERMAN.TTF     ", "C:/Windows/Fonts/JOKERMAN.TTF"));
	fonts.push_back(std::make_tuple("JUICE___.TTF     ", "C:/Windows/Fonts/JUICE___.TTF"));
	fonts.push_back(std::make_tuple("KUNSTLER.TTF     ", "C:/Windows/Fonts/KUNSTLER.TTF"));
	fonts.push_back(std::make_tuple("LATINWD.TTF      ", "C:/Windows/Fonts/LATINWD.TTF"));
	fonts.push_back(std::make_tuple("LBRITE.TTF       ", "C:/Windows/Fonts/LBRITE.TTF"));
	fonts.push_back(std::make_tuple("LBRITED.TTF      ", "C:/Windows/Fonts/LBRITED.TTF"));
	fonts.push_back(std::make_tuple("LBRITEDI.TTF     ", "C:/Windows/Fonts/LBRITEDI.TTF"));
	fonts.push_back(std::make_tuple("LBRITEI.TTF      ", "C:/Windows/Fonts/LBRITEI.TTF"));
	fonts.push_back(std::make_tuple("LCALLIG.TTF      ", "C:/Windows/Fonts/LCALLIG.TTF"));
	fonts.push_back(std::make_tuple("LeelaUIb.ttf     ", "C:/Windows/Fonts/LeelaUIb.ttf"));
	fonts.push_back(std::make_tuple("LEELAWAD.TTF     ", "C:/Windows/Fonts/LEELAWAD.TTF"));
	fonts.push_back(std::make_tuple("LEELAWDB.TTF     ", "C:/Windows/Fonts/LEELAWDB.TTF"));
	fonts.push_back(std::make_tuple("LeelawUI.ttf     ", "C:/Windows/Fonts/LeelawUI.ttf"));
	fonts.push_back(std::make_tuple("LeelUIsl.ttf     ", "C:/Windows/Fonts/LeelUIsl.ttf"));
	fonts.push_back(std::make_tuple("LFAX.TTF         ", "C:/Windows/Fonts/LFAX.TTF"));
	fonts.push_back(std::make_tuple("LFAXD.TTF        ", "C:/Windows/Fonts/LFAXD.TTF"));
	fonts.push_back(std::make_tuple("LFAXDI.TTF       ", "C:/Windows/Fonts/LFAXDI.TTF"));
	fonts.push_back(std::make_tuple("LFAXI.TTF        ", "C:/Windows/Fonts/LFAXI.TTF"));
	fonts.push_back(std::make_tuple("LHANDW.TTF       ", "C:/Windows/Fonts/LHANDW.TTF"));
	fonts.push_back(std::make_tuple("LSANS.TTF        ", "C:/Windows/Fonts/LSANS.TTF"));
	fonts.push_back(std::make_tuple("LSANSD.TTF       ", "C:/Windows/Fonts/LSANSD.TTF"));
	fonts.push_back(std::make_tuple("LSANSDI.TTF      ", "C:/Windows/Fonts/LSANSDI.TTF"));
	fonts.push_back(std::make_tuple("LSANSI.TTF       ", "C:/Windows/Fonts/LSANSI.TTF"));
	fonts.push_back(std::make_tuple("LTYPE.TTF        ", "C:/Windows/Fonts/LTYPE.TTF"));
	fonts.push_back(std::make_tuple("LTYPEB.TTF       ", "C:/Windows/Fonts/LTYPEB.TTF"));
	fonts.push_back(std::make_tuple("LTYPEBO.TTF      ", "C:/Windows/Fonts/LTYPEBO.TTF"));
	fonts.push_back(std::make_tuple("LTYPEO.TTF       ", "C:/Windows/Fonts/LTYPEO.TTF"));
	fonts.push_back(std::make_tuple("lucon.ttf        ", "C:/Windows/Fonts/lucon.ttf"));
	fonts.push_back(std::make_tuple("l_10646.ttf      ", "C:/Windows/Fonts/l_10646.ttf"));
	fonts.push_back(std::make_tuple("MAGNETOB.TTF     ", "C:/Windows/Fonts/MAGNETOB.TTF"));
	fonts.push_back(std::make_tuple("MAIAN.TTF        ", "C:/Windows/Fonts/MAIAN.TTF"));
	fonts.push_back(std::make_tuple("malgun.ttf       ", "C:/Windows/Fonts/malgun.ttf"));
	fonts.push_back(std::make_tuple("malgunbd.ttf     ", "C:/Windows/Fonts/malgunbd.ttf"));
	fonts.push_back(std::make_tuple("malgunsl.ttf     ", "C:/Windows/Fonts/malgunsl.ttf"));
	fonts.push_back(std::make_tuple("marlett.ttf      ", "C:/Windows/Fonts/marlett.ttf"));
	fonts.push_back(std::make_tuple("MATURASC.TTF     ", "C:/Windows/Fonts/MATURASC.TTF"));
	fonts.push_back(std::make_tuple("micross.ttf      ", "C:/Windows/Fonts/micross.ttf"));
	fonts.push_back(std::make_tuple("MISTRAL.TTF      ", "C:/Windows/Fonts/MISTRAL.TTF"));
	fonts.push_back(std::make_tuple("mmrtext.ttf      ", "C:/Windows/Fonts/mmrtext.ttf"));
	fonts.push_back(std::make_tuple("mmrtextb.ttf     ", "C:/Windows/Fonts/mmrtextb.ttf"));
	fonts.push_back(std::make_tuple("MOD20.TTF        ", "C:/Windows/Fonts/MOD20.TTF"));
	fonts.push_back(std::make_tuple("monbaiti.ttf     ", "C:/Windows/Fonts/monbaiti.ttf"));
	fonts.push_back(std::make_tuple("MSUIGHUB.TTF     ", "C:/Windows/Fonts/MSUIGHUB.TTF"));
	fonts.push_back(std::make_tuple("MSUIGHUR.TTF     ", "C:/Windows/Fonts/MSUIGHUR.TTF"));
	fonts.push_back(std::make_tuple("msyi.ttf         ", "C:/Windows/Fonts/msyi.ttf"));
	fonts.push_back(std::make_tuple("MTCORSVA.TTF     ", "C:/Windows/Fonts/MTCORSVA.TTF"));
	fonts.push_back(std::make_tuple("MTEXTRA.TTF      ", "C:/Windows/Fonts/MTEXTRA.TTF"));
	fonts.push_back(std::make_tuple("mvboli.ttf       ", "C:/Windows/Fonts/mvboli.ttf"));
	fonts.push_back(std::make_tuple("NIAGENG.TTF      ", "C:/Windows/Fonts/NIAGENG.TTF"));
	fonts.push_back(std::make_tuple("NIAGSOL.TTF      ", "C:/Windows/Fonts/NIAGSOL.TTF"));
	fonts.push_back(std::make_tuple("Nirmala.ttf      ", "C:/Windows/Fonts/Nirmala.ttf"));
	fonts.push_back(std::make_tuple("NirmalaB.ttf     ", "C:/Windows/Fonts/NirmalaB.ttf"));
	fonts.push_back(std::make_tuple("NirmalaS.ttf     ", "C:/Windows/Fonts/NirmalaS.ttf"));
	fonts.push_back(std::make_tuple("ntailu.ttf       ", "C:/Windows/Fonts/ntailu.ttf"));
	fonts.push_back(std::make_tuple("ntailub.ttf      ", "C:/Windows/Fonts/ntailub.ttf"));
	fonts.push_back(std::make_tuple("OCRAEXT.TTF      ", "C:/Windows/Fonts/OCRAEXT.TTF"));
	fonts.push_back(std::make_tuple("OLDENGL.TTF      ", "C:/Windows/Fonts/OLDENGL.TTF"));
	fonts.push_back(std::make_tuple("ONYX.TTF         ", "C:/Windows/Fonts/ONYX.TTF"));
	fonts.push_back(std::make_tuple("OUTLOOK.TTF      ", "C:/Windows/Fonts/OUTLOOK.TTF"));
	fonts.push_back(std::make_tuple("pala.ttf         ", "C:/Windows/Fonts/pala.ttf"));
	fonts.push_back(std::make_tuple("palab.ttf        ", "C:/Windows/Fonts/palab.ttf"));
	fonts.push_back(std::make_tuple("palabi.ttf       ", "C:/Windows/Fonts/palabi.ttf"));
	fonts.push_back(std::make_tuple("palai.ttf        ", "C:/Windows/Fonts/palai.ttf"));
	fonts.push_back(std::make_tuple("PALSCRI.TTF      ", "C:/Windows/Fonts/PALSCRI.TTF"));
	fonts.push_back(std::make_tuple("PAPYRUS.TTF      ", "C:/Windows/Fonts/PAPYRUS.TTF"));
	fonts.push_back(std::make_tuple("PARCHM.TTF       ", "C:/Windows/Fonts/PARCHM.TTF"));
	fonts.push_back(std::make_tuple("PERBI___.TTF     ", "C:/Windows/Fonts/PERBI___.TTF"));
	fonts.push_back(std::make_tuple("PERB____.TTF     ", "C:/Windows/Fonts/PERB____.TTF"));
	fonts.push_back(std::make_tuple("PERI____.TTF     ", "C:/Windows/Fonts/PERI____.TTF"));
	fonts.push_back(std::make_tuple("PERTIBD.TTF      ", "C:/Windows/Fonts/PERTIBD.TTF"));
	fonts.push_back(std::make_tuple("PERTILI.TTF      ", "C:/Windows/Fonts/PERTILI.TTF"));
	fonts.push_back(std::make_tuple("PER_____.TTF     ", "C:/Windows/Fonts/PER_____.TTF"));
	fonts.push_back(std::make_tuple("phagspa.ttf      ", "C:/Windows/Fonts/phagspa.ttf"));
	fonts.push_back(std::make_tuple("phagspab.ttf     ", "C:/Windows/Fonts/phagspab.ttf"));
	fonts.push_back(std::make_tuple("PLAYBILL.TTF     ", "C:/Windows/Fonts/PLAYBILL.TTF"));
	fonts.push_back(std::make_tuple("POORICH.TTF      ", "C:/Windows/Fonts/POORICH.TTF"));
	fonts.push_back(std::make_tuple("PRISTINA.TTF     ", "C:/Windows/Fonts/PRISTINA.TTF"));
	fonts.push_back(std::make_tuple("RAGE.TTF         ", "C:/Windows/Fonts/RAGE.TTF"));
	fonts.push_back(std::make_tuple("RAVIE.TTF        ", "C:/Windows/Fonts/RAVIE.TTF"));
	fonts.push_back(std::make_tuple("REFSAN.TTF       ", "C:/Windows/Fonts/REFSAN.TTF"));
	fonts.push_back(std::make_tuple("REFSPCL.TTF      ", "C:/Windows/Fonts/REFSPCL.TTF"));
	fonts.push_back(std::make_tuple("ROCCB___.TTF     ", "C:/Windows/Fonts/ROCCB___.TTF"));
	fonts.push_back(std::make_tuple("ROCC____.TTF     ", "C:/Windows/Fonts/ROCC____.TTF"));
	fonts.push_back(std::make_tuple("ROCK.TTF         ", "C:/Windows/Fonts/ROCK.TTF"));
	fonts.push_back(std::make_tuple("ROCKB.TTF        ", "C:/Windows/Fonts/ROCKB.TTF"));
	fonts.push_back(std::make_tuple("ROCKBI.TTF       ", "C:/Windows/Fonts/ROCKBI.TTF"));
	fonts.push_back(std::make_tuple("ROCKEB.TTF       ", "C:/Windows/Fonts/ROCKEB.TTF"));
	fonts.push_back(std::make_tuple("ROCKI.TTF        ", "C:/Windows/Fonts/ROCKI.TTF"));
	fonts.push_back(std::make_tuple("SCHLBKB.TTF      ", "C:/Windows/Fonts/SCHLBKB.TTF"));
	fonts.push_back(std::make_tuple("SCHLBKBI.TTF     ", "C:/Windows/Fonts/SCHLBKBI.TTF"));
	fonts.push_back(std::make_tuple("SCHLBKI.TTF      ", "C:/Windows/Fonts/SCHLBKI.TTF"));
	fonts.push_back(std::make_tuple("SCRIPTBL.TTF     ", "C:/Windows/Fonts/SCRIPTBL.TTF"));
	fonts.push_back(std::make_tuple("segmdl2.ttf      ", "C:/Windows/Fonts/segmdl2.ttf"));
	fonts.push_back(std::make_tuple("SegoeIcons.ttf   ", "C:/Windows/Fonts/SegoeIcons.ttf"));
	fonts.push_back(std::make_tuple("segoepr.ttf      ", "C:/Windows/Fonts/segoepr.ttf"));
	fonts.push_back(std::make_tuple("segoeprb.ttf     ", "C:/Windows/Fonts/segoeprb.ttf"));
	fonts.push_back(std::make_tuple("segoesc.ttf      ", "C:/Windows/Fonts/segoesc.ttf"));
	fonts.push_back(std::make_tuple("segoescb.ttf     ", "C:/Windows/Fonts/segoescb.ttf"));
	fonts.push_back(std::make_tuple("segoeui.ttf      ", "C:/Windows/Fonts/segoeui.ttf"));
	fonts.push_back(std::make_tuple("segoeuib.ttf     ", "C:/Windows/Fonts/segoeuib.ttf"));
	fonts.push_back(std::make_tuple("segoeuii.ttf     ", "C:/Windows/Fonts/segoeuii.ttf"));
	fonts.push_back(std::make_tuple("segoeuil.ttf     ", "C:/Windows/Fonts/segoeuil.ttf"));
	fonts.push_back(std::make_tuple("segoeuisl.ttf    ", "C:/Windows/Fonts/segoeuisl.ttf"));
	fonts.push_back(std::make_tuple("segoeuiz.ttf     ", "C:/Windows/Fonts/segoeuiz.ttf"));
	fonts.push_back(std::make_tuple("seguibl.ttf      ", "C:/Windows/Fonts/seguibl.ttf"));
	fonts.push_back(std::make_tuple("seguibli.ttf     ", "C:/Windows/Fonts/seguibli.ttf"));
	fonts.push_back(std::make_tuple("seguiemj.ttf     ", "C:/Windows/Fonts/seguiemj.ttf"));
	fonts.push_back(std::make_tuple("seguihis.ttf     ", "C:/Windows/Fonts/seguihis.ttf"));
	fonts.push_back(std::make_tuple("seguili.ttf      ", "C:/Windows/Fonts/seguili.ttf"));
	fonts.push_back(std::make_tuple("seguisb.ttf      ", "C:/Windows/Fonts/seguisb.ttf"));
	fonts.push_back(std::make_tuple("seguisbi.ttf     ", "C:/Windows/Fonts/seguisbi.ttf"));
	fonts.push_back(std::make_tuple("seguisli.ttf     ", "C:/Windows/Fonts/seguisli.ttf"));
	fonts.push_back(std::make_tuple("seguisym.ttf     ", "C:/Windows/Fonts/seguisym.ttf"));
	fonts.push_back(std::make_tuple("SegUIVar.ttf     ", "C:/Windows/Fonts/SegUIVar.ttf"));
	fonts.push_back(std::make_tuple("SHOWG.TTF        ", "C:/Windows/Fonts/SHOWG.TTF"));
	fonts.push_back(std::make_tuple("simsunb.ttf      ", "C:/Windows/Fonts/simsunb.ttf"));

	fonts.push_back(std::make_tuple("SitkaVF - Italic.ttf", "C:/Windows/Fonts/SitkaVF - Italic.ttf"));
	fonts.push_back(std::make_tuple("SitkaVF.ttf ", "C:/Windows/Fonts/SitkaVF.ttf"));
	fonts.push_back(std::make_tuple("SNAP____.TTF", "C:/Windows/Fonts/SNAP____.TTF"));
	fonts.push_back(std::make_tuple("STENCIL.TTF ", "C:/Windows/Fonts/STENCIL.TTF"));
	fonts.push_back(std::make_tuple("sylfaen.ttf ", "C:/Windows/Fonts/sylfaen.ttf"));
	fonts.push_back(std::make_tuple("symbol.ttf  ", "C:/Windows/Fonts/symbol.ttf"));
	fonts.push_back(std::make_tuple("tahoma.ttf  ", "C:/Windows/Fonts/tahoma.ttf"));
	fonts.push_back(std::make_tuple("tahomabd.ttf", "C:/Windows/Fonts/tahomabd.ttf"));
	fonts.push_back(std::make_tuple("taile.ttf   ", "C:/Windows/Fonts/taile.ttf"));
	fonts.push_back(std::make_tuple("taileb.ttf  ", "C:/Windows/Fonts/taileb.ttf"));
	fonts.push_back(std::make_tuple("TCBI____.TTF", "C:/Windows/Fonts/TCBI____.TTF"));
	fonts.push_back(std::make_tuple("TCB_____.TTF", "C:/Windows/Fonts/TCB_____.TTF"));
	fonts.push_back(std::make_tuple("TCCB____.TTF", "C:/Windows/Fonts/TCCB____.TTF"));
	fonts.push_back(std::make_tuple("TCCEB.TTF   ", "C:/Windows/Fonts/TCCEB.TTF"));
	fonts.push_back(std::make_tuple("TCCM____.TTF", "C:/Windows/Fonts/TCCM____.TTF"));
	fonts.push_back(std::make_tuple("TCMI____.TTF", "C:/Windows/Fonts/TCMI____.TTF"));
	fonts.push_back(std::make_tuple("TCM_____.TTF", "C:/Windows/Fonts/TCM_____.TTF"));
	fonts.push_back(std::make_tuple("TEMPSITC.TTF", "C:/Windows/Fonts/TEMPSITC.TTF"));
	fonts.push_back(std::make_tuple("times.ttf   ", "C:/Windows/Fonts/times.ttf"));
	fonts.push_back(std::make_tuple("timesbd.ttf ", "C:/Windows/Fonts/timesbd.ttf"));
	fonts.push_back(std::make_tuple("timesbi.ttf ", "C:/Windows/Fonts/timesbi.ttf"));
	fonts.push_back(std::make_tuple("timesi.ttf  ", "C:/Windows/Fonts/timesi.ttf"));
	fonts.push_back(std::make_tuple("trebuc.ttf  ", "C:/Windows/Fonts/trebuc.ttf"));
	fonts.push_back(std::make_tuple("trebucbd.ttf", "C:/Windows/Fonts/trebucbd.ttf"));
	fonts.push_back(std::make_tuple("trebucbi.ttf", "C:/Windows/Fonts/trebucbi.ttf"));
	fonts.push_back(std::make_tuple("trebucit.ttf", "C:/Windows/Fonts/trebucit.ttf"));
	fonts.push_back(std::make_tuple("verdana.ttf ", "C:/Windows/Fonts/verdana.ttf"));
	fonts.push_back(std::make_tuple("verdanab.ttf", "C:/Windows/Fonts/verdanab.ttf"));
	fonts.push_back(std::make_tuple("verdanai.ttf", "C:/Windows/Fonts/verdanai.ttf"));
	fonts.push_back(std::make_tuple("verdanaz.ttf", "C:/Windows/Fonts/verdanaz.ttf"));
	fonts.push_back(std::make_tuple("VINERITC.TTF", "C:/Windows/Fonts/VINERITC.TTF"));
	fonts.push_back(std::make_tuple("VIVALDII.TTF", "C:/Windows/Fonts/VIVALDII.TTF"));
	fonts.push_back(std::make_tuple("VLADIMIR.TTF", "C:/Windows/Fonts/VLADIMIR.TTF"));
	fonts.push_back(std::make_tuple("webdings.ttf", "C:/Windows/Fonts/webdings.ttf"));
	fonts.push_back(std::make_tuple("wingding.ttf", "C:/Windows/Fonts/wingding.ttf"));
	fonts.push_back(std::make_tuple("WINGDNG2.TTF", "C:/Windows/Fonts/WINGDNG2.TTF"));
	fonts.push_back(std::make_tuple("WINGDNG3.TTF", "C:/Windows/Fonts/WINGDNG3.TTF"));


}
